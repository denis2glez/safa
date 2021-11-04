#pragma once

#include <vector>
#include <cstdio>
#include <algorithm>

#include <itkObject.h>
#include <itkObjectFactory.h>
#include <itkProcessObject.h>
#include "Common/itkImageTypes.h"

using namespace std;

namespace AF4D {

/**********************************************************************************************
 * Algunas clases y definiciones auxiliares.
 **********************************************************************************************/
class Point {
public:

	int x, y, z;

	Point(int xx = 0, int yy = 0, int zz = 0) :
		x(xx), y(yy), z(zz) {
	}

	friend bool operator<(const Point &a, const Point &b) {
		return a.y > b.y;
	}
};

class Edge {
public:

	int from, to, inverse;

	vector<int> neighbors;

	Edge(int i = 0, int j = 0, int inv = 0, vector<int> n = vector<int> ()) {
		from = i;
		to = j;
		inverse = inv;
		neighbors = n;
	}

};

class Path {
public:

	int extreme;

	vector<int> p;

	Path(int e = 0, vector<int> v = vector<int> ()) {
		extreme = e;
		p = v;
	}

};

typedef vector<Point> PointVector;
typedef vector<Edge> EdgeVector;

/**********************************************************************************************
 * Clase principal.
 * Se crea un objeto de tipo CenterLine pasando como parámetro la nube de puntos (PointVector &cloud)
 * Luego se invoca el método FindCenterLine() para obtener la salida.
 **********************************************************************************************/
class CenterLine: public itk::Object {
public:
	/** Standard clas typedefs. */
	typedef CenterLine Self;
	typedef itk::Object Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro( Self );
	itkTypeMacro( Self, Superclass );

	/* Devuelve el camino con índice num */
	vector<int> GetPathWithIndex(int num);

	/* Establece la imagen esqueletonizada */
	void SetInput(Scalar3DImageType::Pointer img);

	/* Encuentra la línea central */
	vector<PointVector> FindCenterLine(int points_number);

	/* Función utilitaria que escribe en un fichero una nube de puntos dada */
	static void PointsCloudToFile(PointVector &pv, const char *fileName);

protected:
	CenterLine() {
	}

	virtual ~CenterLine() {
		delete[] _points;
		delete[] _maxPaths;
	}

private:

	Scalar3DImageType::Pointer _inputImg;

	static const int MAX_NUM_POINTS = 1000;

	static const int NUM_PATHS = 10;

	int _N, _pathMinCost;

	Point *_points;

	Path *_maxPaths;

	EdgeVector _edges;

	void SetCloud(PointVector &cloud);

	inline int MAX3(int x, int y, int z);

	inline int ABS(int a);

	int IndexEdge(int from, int to, EdgeVector &edges);

	int FindEdgePos(int from, int to, EdgeVector &edges);

	bool IsNeighbor(const Point &a, const Point &b);

	int BinarySearch(int from, EdgeVector &edges);

	inline int CalculateCost(const vector<int> &p);

	void FindPlace(int to, vector<int> &p);

	bool CreateCycle(int index, vector<int> &v);

	vector<int> AnalyzePaths(vector<int> v, int index);

	void BuildGraph();

};

}
