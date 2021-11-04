#ifndef FORM3D_H_
#define FORM3D_H_

#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#include "ui_Form3D.h"

class Form3D : public QDialog, public Ui::Dialog {
Q_OBJECT
public:
	Form3D();
	virtual ~Form3D();

	void Set3DImage(vtkImageData *vtkImage);
	void SetRenderWindow();
	void FlipImage();

public slots:
	void Show3DView(bool b);

private:
	vtkVolume *volume;
	vtkRenderer *renderer;
};

#endif /* FORM3D_H_ */
