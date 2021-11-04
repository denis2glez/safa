#include "Segmentation/AF4DCenterLine.h"
#include <itkImageRegionConstIteratorWithIndex.h>
#include "Segmentation/itkBinaryThinningImageFilter3D.h"

namespace AF4D {
/**********************************************************************************************
 * Funciones utilitarias.
 **********************************************************************************************/
inline int CenterLine::MAX3(int x, int y, int z) {
	int tmp;
	if (x > y)
		tmp = x;
	else
		tmp = y;
	if (tmp > z)
		return tmp;
	else
		return z;
}
inline int CenterLine::ABS(int a) {
	if (a > 0)
		return a;
	else
		return (-a);
}
/**********************************************************************************************
 * Método que establece la imagen 3D de entrada y que aplica el filtro
 * BinaryThinningImageFilter3D para 'suavizar' la imagen original.
 **********************************************************************************************/
void CenterLine::SetInput(Scalar3DImageType::Pointer img) {
	//Typedef stuff
	typedef Scalar3DImageType::IndexType IndexType;
	typedef itk::BinaryThinningImageFilter3D<Scalar3DImageType,
			Scalar3DImageType> FilterThinningType;

	if (_inputImg != img) {
		//Crear el filtro para obtener el skeleton
		FilterThinningType::Pointer thinFilter = FilterThinningType::New();
		thinFilter->SetInput(img);
		thinFilter->Update();

		//Obtener la imagen thinning
		Scalar3DImageType::Pointer thinImg = thinFilter->GetOutput();

		//Obtener la nube de puntos
		itk::ImageRegionConstIteratorWithIndex<Scalar3DImageType> it(thinImg,
				thinImg->GetRequestedRegion());

		PointVector cloud;

		it.GoToBegin();
		while (!it.IsAtEnd()) {
			const PixelType value = it.Get();
			if (value == 1.0) {
				IndexType index = it.GetIndex();
				Point p(index[0], index[1], index[2]);
				cloud.push_back(p);
			}
			++it;
		}

		_inputImg = img;

		//Establecer la nube de puntos construida
		SetCloud(cloud);
	}

}
/**********************************************************************************************
 * Método que recibe la nube de puntos.
 * Inicializa algunos atributos e invoca a BuildGraph donde se inicializa _edges.
 **********************************************************************************************/
void CenterLine::SetCloud(PointVector &cloud) {
	_N = cloud.size();
	_pathMinCost = -1;
	_points = new Point[MAX_NUM_POINTS];
	_maxPaths = new Path[NUM_PATHS];
	for (int i = 0; i < _N; i++)
		_points[i] = cloud[i];

	BuildGraph();

}
/**********************************************************************************************
 * Método que determina si dos ptos dados se consideran vecinos o no.
 **********************************************************************************************/
bool CenterLine::IsNeighbor(const Point&a, const Point&b) {
	return (MAX3(ABS(a.x - b.x), ABS(a.y - b.y), ABS(a.z - b.z)) <= 1);
}
/**********************************************************************************************
 * Método que devuelve la posición de la arista (from-to) dentro del grafo pasado como parámetro.
 * Devuelve -1 en caso de no aparecer.
 **********************************************************************************************/
int CenterLine::IndexEdge(int from, int to, EdgeVector &edges) {
	int pos = edges.size() - 1;
	while (pos >= 0 && edges[pos].from == from) {
		if (edges[pos].to == to)
			return pos;
		pos--;
	}
	return -1;
}
/**********************************************************************************************
 * Método que realiza una búsqueda binaria de la primera arista que sale de from en el grafo
 * pasado como parámetro.
 **********************************************************************************************/
int CenterLine::BinarySearch(int from, EdgeVector &edges) {
	int i = 0, j = edges.size() - 1, index;
	if (j < 0)
		return 0;
	while (i <= j) {
		index = (i + j) / 2;
		if (edges[index].from == from)
			break;
		if (edges[index].from < from)
			i = index + 1;
		else
			j = index - 1;
	}
	while (index > 0 && edges[index - 1].from == from)
		index--;
	return index;
}
/**********************************************************************************************
 * Método que encuentra la posición de una arista (from-to) dentro del vector de aristas (grafo)
 * pasado como parámetro. Usa la búsqueda binaria.
 **********************************************************************************************/
int CenterLine::FindEdgePos(int from, int to, EdgeVector &edges) {
	int index = BinarySearch(from, edges);
	while (edges[index].to != to)
		index++;
	return index;
}
/**********************************************************************************************
 * Método donde se inicializa Edges. Se construye un grafo inicial edges que luego se reduce
 * para lograr un mejor desempeño.
 **********************************************************************************************/
void CenterLine::BuildGraph() {
	vector<int> edges[MAX_NUM_POINTS];
	EdgeVector edgesTmp;

//	Ordenamos según la coordenada 'y' (no es un orden total)
	sort(_points, _points + _N);

//	Construimos un grafo no dirigido donde 2 nodos son vecinos si IsNeighbor devuelve true
	int i, j;
	for (i = 0, j = 0; i < _N; i++) {
		j = i + 1;
		while (j < _N && (ABS(_points[i].y - _points[j].y) <= 1)) {
			if (IsNeighbor(_points[i], _points[j])) {
				edges[i].push_back(j);
				edges[j].push_back(i);
			}
			j++;
		}
	}

//	Reducimos el grafo anterior considerando solamente los nodos que tienen 1 o más de 2
//	vecinos. Evitando obtener aristas dobles, en caso de obtener dos aristas paralelas nos
//	quedamos con la de mayor costo.
	for (i = 0; i < _N; i++) {
		if (edges[i].size() != 2) {
			for (int k = 0; k < edges[i].size(); k++) {
				int start = i; // Parte desde i == start
				int suc = edges[i][k]; // Termina en suc
				vector<int> path;
				Edge n;
				while (edges[suc].size() == 2) {
					path.push_back(suc);
					if (edges[suc][0] == start) {
						start = suc;
						suc = edges[suc][1];
					} else {
						start = suc;
						suc = edges[suc][0];
					}
				}
				n.from = i;
				n.to = suc;
				n.neighbors = path;
				int pos = IndexEdge(i, suc, edgesTmp);
				if (pos != -1) {
					if (edgesTmp[pos].neighbors.size() < path.size())
						edgesTmp[pos] = n;
					// El costo de la existente es mayor que la nueva Edge
					// Simplemente no la tenemos en cuenta
				} else
					edgesTmp.push_back(n);
			}
		}
	}
//	Luego agregamos a cada arista del grafo reducido (Edges_tmp) la información referente
//	al índice de la arista inversa para luego reducir el tiempo de procesado (preproceso)
	const int size = edgesTmp.size();
	for (int n = 0; n < size; n++) {
		int ind = FindEdgePos(edgesTmp[n].to, edgesTmp[n].from, edgesTmp);
		edgesTmp[n].inverse = ind;
		edgesTmp[ind].inverse = n;
	}
	vector<bool> mark(size);
	fill(mark.begin(), mark.end(), false);
	for (int i = 0; i < size; i++)
		if (edgesTmp[i].neighbors.size() == 0)
			mark[i] = true;
	for (int i = 0; i < size; i++) {
		if (mark[i] && edgesTmp[i].from < edgesTmp[i].to) {
			for (int j = 0; j < size; j++) {
				if (!mark[j] && edgesTmp[j].from == edgesTmp[i].to)
					edgesTmp[j].from = edgesTmp[i].from;
				if (!mark[j] && edgesTmp[j].to == edgesTmp[i].to)
					edgesTmp[j].to = edgesTmp[i].from;
			}
		}
	}
	for (int i = 0; i < size; i++)
		if (!mark[i])
			_edges.push_back(edgesTmp[i]);
}
/**********************************************************************************************
 * Método que devuelve el costo del camino pasado como parámetro.
 **********************************************************************************************/
inline int CenterLine::CalculateCost(const vector<int> &p) {
	int cost = 0;
	for (int i = 0; i < p.size(); i++)
		cost += _edges[p[i]].neighbors.size() + 1;
	return cost;
}
/**********************************************************************************************
 * Método que encuentra el lugar del camino que es pasado como parámetro (to: fin del camino
 * y p: nodos visitados a partir del origen hasta el fin) dentro de los caminos en max_Paths.
 **********************************************************************************************/
void CenterLine::FindPlace(int to, vector<int> &p) {
	Path v(to, p), tmp, tmp1;
	int the_cost = CalculateCost(v.p);
	for (int i = 0; i < NUM_PATHS; i++) {
		if (CalculateCost(_maxPaths[i].p) <= the_cost) {
			if (_maxPaths[i].p == v.p && _maxPaths[i].extreme == v.extreme)
				return;
			tmp = _maxPaths[i];
			_maxPaths[i] = v;
			if (i < NUM_PATHS - 1)
				i++;
			else {
				_pathMinCost = CalculateCost(_maxPaths[NUM_PATHS - 1].p);
				return;
			}
			while (i < NUM_PATHS - 1) {
				tmp1 = _maxPaths[i];
				_maxPaths[i] = tmp;
				tmp = tmp1;
				i++;
			}
			_maxPaths[i] = tmp;
			_pathMinCost = CalculateCost(_maxPaths[NUM_PATHS - 1].p);
			return;
		}
	}
}
/**********************************************************************************************
 * Método que devuelve true si el camino especificado por el vector de ptos contiene un ciclo,
 * devuelve false en caso contrario.
 **********************************************************************************************/
bool CenterLine::CreateCycle(int index, vector<int> &v) {
	int from, to;
	int i_to = _edges[index].to;
	int inverse = _edges[index].inverse;
	for (int i = 0; i < v.size(); i++) {
		from = _edges[v[i]].from;
		to = _edges[v[i]].to;
		if (v[i] == index || v[i] == inverse || from == i_to || to == i_to)
			return true;
	}
	return false;
}
/**********************************************************************************************
 * Método recursivo que recorre todos los caminos que comienzan en el primer pto (points[0]),
 * guarda los que tengan una mayor longitud en la variable max_Paths (método FindPlace) y mantiene
 * en path_min_cost el menor costo de los caminos guardados.
 * NOTA: Este riterio se aplica luego de reducir el grafo original en el método BuildGraph.
 **********************************************************************************************/
vector<int> CenterLine::AnalyzePaths(vector<int> v, int index) {
	vector<int> result;
	int cost = -1;
	int to = _edges[index].to;
	v.push_back(index);
	int next = BinarySearch(to, _edges);
	while (next < _edges.size() && _edges[next].from == to) {
		if (!CreateCycle(next, v)) {
			vector<int> tmp = AnalyzePaths(v, next);
			int p_cost = CalculateCost(tmp);
			if (p_cost > cost) {
				cost = p_cost;
				result.clear();
				result.push_back(index);
				for (int i = 0; i < tmp.size(); i++)
					result.push_back(tmp[i]);
			}
		}
		next++;
	}
	if (CalculateCost(v) > _pathMinCost)
		FindPlace(_edges[index].to, v);
	return result;
}
/**********************************************************************************************
 * Método que devuelve un vector de índices que representan el num-ésimo camino.
 * Si no existe el camino devuelve un vector vacío.
 **********************************************************************************************/
vector<int> CenterLine::GetPathWithIndex(int num) {
	vector<int> result;
	if (_maxPaths[num].p.size() == 0)
		return result;
	int size = _maxPaths[num].p.size();
	int now = 0, i;
	for (i = 0; i < size - 1; i++) {
		now = _maxPaths[num].p[i];
		result.push_back(_edges[now].from);
		for (int j = 0; j < _edges[now].neighbors.size(); j++)
			result.push_back(_edges[now].neighbors[j]);
	}
	now = _maxPaths[num].p[i];
	result.push_back(_edges[now].from);
	for (int j = 0; j < _edges[now].neighbors.size(); j++)
		result.push_back(_edges[now].neighbors[j]);
	result.push_back(_edges[now].to);
	return result;
}
/**********************************************************************************************
 * Método público principal que devuelve la línea central de la aorta con la cantidad de puntos
 * especificada por points_number, encontrada a partir de la nube de entrada.
 **********************************************************************************************/
vector<PointVector> CenterLine::FindCenterLine(int points_number) {
	vector<PointVector> result(NUM_PATHS);
	vector<int> s;
	AnalyzePaths(s, 0);
	vector<int> path;
	int size, step, rest;
	for (int i = 0; i < NUM_PATHS; i++) {
		path = GetPathWithIndex(i);
		size = path.size();
		if (points_number < size) {
			step = (size - 1) / (points_number - 1);
			rest = (size - 1) % (points_number - 1);
		} else {
			step = 1;
			rest = 0;
		}
		int k = 0;
		for (int j = 0; k < points_number && j < size; j += step, k++) {
			if (rest > 0 && j < size - 1) {
				rest--;
				j++;
			}
			result[i].push_back(_points[path[j]]);

		}
	}
	return result;
}
/**********************************************************************************************
 * Método que toma como entrada un vector de ptos (nube) y un nombre de archivo, y escribe los
 * ptos (un punto por línea: x y z) en el archivo.
 **********************************************************************************************/
void CenterLine::PointsCloudToFile(PointVector &pv, const char *fileName) {
	FILE *fout = fopen(fileName, "w");
	int size = pv.size();
	for (int i = 0; i < size; i++)
		fprintf(fout, "%d %d %d\n", pv[i].x, pv[i].y, pv[i].z);

	fclose(fout);
}
}
