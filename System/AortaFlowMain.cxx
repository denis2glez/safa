/** \file AortaFlowMain.cxx
 * Este es el punto de entrada a la aplicacion.
 */

/*Standard includes*/
#include <cassert>
/*Qt includes*/
#include <QApplication>
#include <QCleanlooksStyle>
#include <QPlastiqueStyle>
#include <QMacStyle>         //Solo disponible para Mac OS X
#include <QMotifStyle>
#include <QCDEStyle>

/*Local includes*/
//#include "DICOMVectorial3DImageLoader.h"
//#include "DICOMScalar4DImageLoader.h"
//#include "DICOMVectorial4DImageLoader.h"
//#include "itkReaderEvents.h"

#include "GUI/AortaFlowApp.h"


extern int qInitResources_ResourcesFile();


int main(int argc, char** argv){

	QApplication app(argc, argv);

//	QApplication::setStyle(new QPlastiqueStyle);

	qInitResources_ResourcesFile();

	AortaFlowApp mainwindow;
	mainwindow.show();
	return app.exec();
}
