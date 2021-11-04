/********************************************************************************
** Form generated from reading UI file 'AortaFlowApp.ui'
**
** Created: Fri 15. Jun 07:41:29 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AORTAFLOWAPP_H
#define UI_AORTAFLOWAPP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_AortaFlowApp
{
public:
    QAction *actionQuit;
    QAction *actionRecent_studies;
    QAction *actionSuperior_frame;
    QAction *actionInferior_frame;
    QAction *actionProperties_window;
    QAction *actionLoad_DICOM_series;
    QAction *actionLoad_DICOM_vectorial_serie;
    QAction *actionSyncronize_both_frames;
    QAction *actionFull_screen;
    QAction *actionNormal;
    QAction *action3D_View;
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QAction *actionClose_current_serie;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frameSup;
    QGridLayout *gridLayout_8;
    QFrame *frameSagitalSup;
    QGridLayout *gridLayout_5;
    QLabel *labelSag1_2;
    QVTKWidget *qvtkWidgetSag1;
    QLabel *labelSag1_1;
    QLabel *labelSag1_3;
    QSlider *sliderSag1;
    QFrame *frameAxialSup;
    QGridLayout *gridLayout_6;
    QLabel *labelAxi1_2;
    QVTKWidget *qvtkWidgetAxi1;
    QLabel *labelAxi1_3;
    QLabel *labelAxi1_1;
    QSlider *sliderAxi1;
    QFrame *frameCoronalSup;
    QGridLayout *gridLayout_7;
    QLabel *labelCor1_2;
    QVTKWidget *qvtkWidgetCor1;
    QLabel *labelCor1_3;
    QLabel *labelCor1_1;
    QSlider *sliderCor1;
    QFrame *frameInf;
    QGridLayout *gridLayout_12;
    QFrame *frameSagitalInf;
    QGridLayout *gridLayout_9;
    QLabel *labelSag2_2;
    QVTKWidget *qvtkWidgetSag2;
    QLabel *labelSag2_3;
    QLabel *labelSag2_1;
    QSlider *sliderSag2;
    QFrame *frameAxialInf;
    QGridLayout *gridLayout_11;
    QLabel *labelAxi2_2;
    QVTKWidget *qvtkWidgetAxi2;
    QLabel *labelAxi2_3;
    QLabel *labelAxi2_1;
    QSlider *sliderAxi2;
    QFrame *frameCoronalInf;
    QGridLayout *gridLayout_10;
    QLabel *labelCor2_2;
    QVTKWidget *qvtkWidgetCor2;
    QLabel *labelCor2_3;
    QLabel *labelCor2_1;
    QSlider *sliderCor2;
    QProgressBar *progressBar;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QDockWidget *dockWidget_Recent_studies;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QDockWidget *dockWidget_Properties_window;
    QWidget *dockWidgetContents_2;
    QFormLayout *formLayout;
    QLabel *labelTimeStep;
    QSpinBox *spinBox;
    QLabel *labelVelocityComponent;
    QLabel *labelSagSlice;
    QLineEdit *lineEdit;
    QLabel *labelAxiSlice;
    QLineEdit *lineEdit_2;
    QLabel *labelCorSlice;
    QLineEdit *lineEdit_3;
    QSpinBox *spinBox_2;

    void setupUi(QMainWindow *AortaFlowApp)
    {
        if (AortaFlowApp->objectName().isEmpty())
            AortaFlowApp->setObjectName(QString::fromUtf8("AortaFlowApp"));
        AortaFlowApp->resize(832, 578);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/icono.png"), QSize(), QIcon::Normal, QIcon::Off);
        AortaFlowApp->setWindowIcon(icon);
        AortaFlowApp->setDocumentMode(false);
        actionQuit = new QAction(AortaFlowApp);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuit->setIcon(icon1);
        actionRecent_studies = new QAction(AortaFlowApp);
        actionRecent_studies->setObjectName(QString::fromUtf8("actionRecent_studies"));
        actionRecent_studies->setCheckable(true);
        actionRecent_studies->setChecked(true);
        actionSuperior_frame = new QAction(AortaFlowApp);
        actionSuperior_frame->setObjectName(QString::fromUtf8("actionSuperior_frame"));
        actionSuperior_frame->setCheckable(false);
        actionSuperior_frame->setChecked(false);
        actionSuperior_frame->setEnabled(false);
        actionInferior_frame = new QAction(AortaFlowApp);
        actionInferior_frame->setObjectName(QString::fromUtf8("actionInferior_frame"));
        actionInferior_frame->setCheckable(false);
        actionInferior_frame->setChecked(false);
        actionInferior_frame->setEnabled(false);
        actionProperties_window = new QAction(AortaFlowApp);
        actionProperties_window->setObjectName(QString::fromUtf8("actionProperties_window"));
        actionProperties_window->setCheckable(true);
        actionProperties_window->setChecked(true);
        actionLoad_DICOM_series = new QAction(AortaFlowApp);
        actionLoad_DICOM_series->setObjectName(QString::fromUtf8("actionLoad_DICOM_series"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_DICOM_series->setIcon(icon2);
        actionLoad_DICOM_vectorial_serie = new QAction(AortaFlowApp);
        actionLoad_DICOM_vectorial_serie->setObjectName(QString::fromUtf8("actionLoad_DICOM_vectorial_serie"));
        actionLoad_DICOM_vectorial_serie->setIcon(icon2);
        actionSyncronize_both_frames = new QAction(AortaFlowApp);
        actionSyncronize_both_frames->setObjectName(QString::fromUtf8("actionSyncronize_both_frames"));
        actionSyncronize_both_frames->setCheckable(true);
        actionSyncronize_both_frames->setChecked(false);
        actionFull_screen = new QAction(AortaFlowApp);
        actionFull_screen->setObjectName(QString::fromUtf8("actionFull_screen"));
        actionNormal = new QAction(AortaFlowApp);
        actionNormal->setObjectName(QString::fromUtf8("actionNormal"));
        action3D_View = new QAction(AortaFlowApp);
        action3D_View->setObjectName(QString::fromUtf8("action3D_View"));
        action3D_View->setCheckable(false);
        actionAbout = new QAction(AortaFlowApp);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout_Qt = new QAction(AortaFlowApp);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionClose_current_serie = new QAction(AortaFlowApp);
        actionClose_current_serie->setObjectName(QString::fromUtf8("actionClose_current_serie"));
        centralwidget = new QWidget(AortaFlowApp);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frameSup = new QFrame(centralwidget);
        frameSup->setObjectName(QString::fromUtf8("frameSup"));
        frameSup->setEnabled(true);
        frameSup->setFrameShape(QFrame::StyledPanel);
        frameSup->setFrameShadow(QFrame::Raised);
        gridLayout_8 = new QGridLayout(frameSup);
        gridLayout_8->setSpacing(0);
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        frameSagitalSup = new QFrame(frameSup);
        frameSagitalSup->setObjectName(QString::fromUtf8("frameSagitalSup"));
        frameSagitalSup->setFrameShape(QFrame::StyledPanel);
        frameSagitalSup->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frameSagitalSup);
        gridLayout_5->setSpacing(0);
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        labelSag1_2 = new QLabel(frameSagitalSup);
        labelSag1_2->setObjectName(QString::fromUtf8("labelSag1_2"));
        labelSag1_2->setMaximumSize(QSize(16777215, 14));

        gridLayout_5->addWidget(labelSag1_2, 0, 3, 1, 1);

        qvtkWidgetSag1 = new QVTKWidget(frameSagitalSup);
        qvtkWidgetSag1->setObjectName(QString::fromUtf8("qvtkWidgetSag1"));

        gridLayout_5->addWidget(qvtkWidgetSag1, 1, 0, 1, 4);

        labelSag1_1 = new QLabel(frameSagitalSup);
        labelSag1_1->setObjectName(QString::fromUtf8("labelSag1_1"));
        labelSag1_1->setMaximumSize(QSize(16777215, 14));

        gridLayout_5->addWidget(labelSag1_1, 0, 0, 1, 1);

        labelSag1_3 = new QLabel(frameSagitalSup);
        labelSag1_3->setObjectName(QString::fromUtf8("labelSag1_3"));
        labelSag1_3->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_5->addWidget(labelSag1_3, 2, 3, 1, 1);

        sliderSag1 = new QSlider(frameSagitalSup);
        sliderSag1->setObjectName(QString::fromUtf8("sliderSag1"));
        sliderSag1->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(sliderSag1, 2, 0, 1, 1);


        gridLayout_8->addWidget(frameSagitalSup, 0, 0, 1, 1);

        frameAxialSup = new QFrame(frameSup);
        frameAxialSup->setObjectName(QString::fromUtf8("frameAxialSup"));
        frameAxialSup->setFrameShape(QFrame::StyledPanel);
        frameAxialSup->setFrameShadow(QFrame::Raised);
        gridLayout_6 = new QGridLayout(frameAxialSup);
        gridLayout_6->setSpacing(0);
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        labelAxi1_2 = new QLabel(frameAxialSup);
        labelAxi1_2->setObjectName(QString::fromUtf8("labelAxi1_2"));
        labelAxi1_2->setMaximumSize(QSize(16777215, 14));

        gridLayout_6->addWidget(labelAxi1_2, 0, 2, 1, 1);

        qvtkWidgetAxi1 = new QVTKWidget(frameAxialSup);
        qvtkWidgetAxi1->setObjectName(QString::fromUtf8("qvtkWidgetAxi1"));

        gridLayout_6->addWidget(qvtkWidgetAxi1, 1, 0, 1, 3);

        labelAxi1_3 = new QLabel(frameAxialSup);
        labelAxi1_3->setObjectName(QString::fromUtf8("labelAxi1_3"));

        gridLayout_6->addWidget(labelAxi1_3, 2, 2, 1, 1);

        labelAxi1_1 = new QLabel(frameAxialSup);
        labelAxi1_1->setObjectName(QString::fromUtf8("labelAxi1_1"));
        labelAxi1_1->setMaximumSize(QSize(16777215, 14));

        gridLayout_6->addWidget(labelAxi1_1, 0, 0, 1, 1);

        sliderAxi1 = new QSlider(frameAxialSup);
        sliderAxi1->setObjectName(QString::fromUtf8("sliderAxi1"));
        sliderAxi1->setOrientation(Qt::Horizontal);

        gridLayout_6->addWidget(sliderAxi1, 2, 0, 1, 1);


        gridLayout_8->addWidget(frameAxialSup, 0, 1, 1, 1);

        frameCoronalSup = new QFrame(frameSup);
        frameCoronalSup->setObjectName(QString::fromUtf8("frameCoronalSup"));
        frameCoronalSup->setFrameShape(QFrame::StyledPanel);
        frameCoronalSup->setFrameShadow(QFrame::Raised);
        gridLayout_7 = new QGridLayout(frameCoronalSup);
        gridLayout_7->setSpacing(0);
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        labelCor1_2 = new QLabel(frameCoronalSup);
        labelCor1_2->setObjectName(QString::fromUtf8("labelCor1_2"));
        labelCor1_2->setMaximumSize(QSize(16777215, 14));

        gridLayout_7->addWidget(labelCor1_2, 0, 2, 1, 1);

        qvtkWidgetCor1 = new QVTKWidget(frameCoronalSup);
        qvtkWidgetCor1->setObjectName(QString::fromUtf8("qvtkWidgetCor1"));

        gridLayout_7->addWidget(qvtkWidgetCor1, 1, 0, 1, 3);

        labelCor1_3 = new QLabel(frameCoronalSup);
        labelCor1_3->setObjectName(QString::fromUtf8("labelCor1_3"));

        gridLayout_7->addWidget(labelCor1_3, 2, 2, 1, 1);

        labelCor1_1 = new QLabel(frameCoronalSup);
        labelCor1_1->setObjectName(QString::fromUtf8("labelCor1_1"));
        labelCor1_1->setMaximumSize(QSize(16777215, 14));

        gridLayout_7->addWidget(labelCor1_1, 0, 0, 1, 1);

        sliderCor1 = new QSlider(frameCoronalSup);
        sliderCor1->setObjectName(QString::fromUtf8("sliderCor1"));
        sliderCor1->setOrientation(Qt::Horizontal);

        gridLayout_7->addWidget(sliderCor1, 2, 0, 1, 1);


        gridLayout_8->addWidget(frameCoronalSup, 0, 2, 1, 1);


        gridLayout->addWidget(frameSup, 0, 0, 1, 1);

        frameInf = new QFrame(centralwidget);
        frameInf->setObjectName(QString::fromUtf8("frameInf"));
        frameInf->setFrameShape(QFrame::StyledPanel);
        frameInf->setFrameShadow(QFrame::Raised);
        gridLayout_12 = new QGridLayout(frameInf);
        gridLayout_12->setSpacing(0);
        gridLayout_12->setContentsMargins(0, 0, 0, 0);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        frameSagitalInf = new QFrame(frameInf);
        frameSagitalInf->setObjectName(QString::fromUtf8("frameSagitalInf"));
        frameSagitalInf->setFrameShape(QFrame::StyledPanel);
        frameSagitalInf->setFrameShadow(QFrame::Raised);
        gridLayout_9 = new QGridLayout(frameSagitalInf);
        gridLayout_9->setSpacing(0);
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        labelSag2_2 = new QLabel(frameSagitalInf);
        labelSag2_2->setObjectName(QString::fromUtf8("labelSag2_2"));
        labelSag2_2->setMaximumSize(QSize(16777215, 14));

        gridLayout_9->addWidget(labelSag2_2, 0, 1, 1, 1);

        qvtkWidgetSag2 = new QVTKWidget(frameSagitalInf);
        qvtkWidgetSag2->setObjectName(QString::fromUtf8("qvtkWidgetSag2"));

        gridLayout_9->addWidget(qvtkWidgetSag2, 1, 0, 1, 2);

        labelSag2_3 = new QLabel(frameSagitalInf);
        labelSag2_3->setObjectName(QString::fromUtf8("labelSag2_3"));

        gridLayout_9->addWidget(labelSag2_3, 2, 1, 1, 1);

        labelSag2_1 = new QLabel(frameSagitalInf);
        labelSag2_1->setObjectName(QString::fromUtf8("labelSag2_1"));
        labelSag2_1->setMaximumSize(QSize(16777215, 14));

        gridLayout_9->addWidget(labelSag2_1, 0, 0, 1, 1);

        sliderSag2 = new QSlider(frameSagitalInf);
        sliderSag2->setObjectName(QString::fromUtf8("sliderSag2"));
        sliderSag2->setOrientation(Qt::Horizontal);

        gridLayout_9->addWidget(sliderSag2, 2, 0, 1, 1);


        gridLayout_12->addWidget(frameSagitalInf, 0, 0, 1, 1);

        frameAxialInf = new QFrame(frameInf);
        frameAxialInf->setObjectName(QString::fromUtf8("frameAxialInf"));
        frameAxialInf->setFrameShape(QFrame::StyledPanel);
        frameAxialInf->setFrameShadow(QFrame::Raised);
        gridLayout_11 = new QGridLayout(frameAxialInf);
        gridLayout_11->setSpacing(0);
        gridLayout_11->setContentsMargins(0, 0, 0, 0);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        labelAxi2_2 = new QLabel(frameAxialInf);
        labelAxi2_2->setObjectName(QString::fromUtf8("labelAxi2_2"));
        labelAxi2_2->setMaximumSize(QSize(16777215, 14));

        gridLayout_11->addWidget(labelAxi2_2, 0, 2, 1, 1);

        qvtkWidgetAxi2 = new QVTKWidget(frameAxialInf);
        qvtkWidgetAxi2->setObjectName(QString::fromUtf8("qvtkWidgetAxi2"));

        gridLayout_11->addWidget(qvtkWidgetAxi2, 1, 0, 1, 3);

        labelAxi2_3 = new QLabel(frameAxialInf);
        labelAxi2_3->setObjectName(QString::fromUtf8("labelAxi2_3"));

        gridLayout_11->addWidget(labelAxi2_3, 2, 2, 1, 1);

        labelAxi2_1 = new QLabel(frameAxialInf);
        labelAxi2_1->setObjectName(QString::fromUtf8("labelAxi2_1"));
        labelAxi2_1->setMaximumSize(QSize(16777215, 14));

        gridLayout_11->addWidget(labelAxi2_1, 0, 0, 1, 1);

        sliderAxi2 = new QSlider(frameAxialInf);
        sliderAxi2->setObjectName(QString::fromUtf8("sliderAxi2"));
        sliderAxi2->setOrientation(Qt::Horizontal);

        gridLayout_11->addWidget(sliderAxi2, 2, 0, 1, 1);


        gridLayout_12->addWidget(frameAxialInf, 0, 1, 1, 1);

        frameCoronalInf = new QFrame(frameInf);
        frameCoronalInf->setObjectName(QString::fromUtf8("frameCoronalInf"));
        frameCoronalInf->setFrameShape(QFrame::StyledPanel);
        frameCoronalInf->setFrameShadow(QFrame::Raised);
        gridLayout_10 = new QGridLayout(frameCoronalInf);
        gridLayout_10->setSpacing(0);
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        labelCor2_2 = new QLabel(frameCoronalInf);
        labelCor2_2->setObjectName(QString::fromUtf8("labelCor2_2"));
        labelCor2_2->setMaximumSize(QSize(16777215, 14));
        labelCor2_2->setStyleSheet(QString::fromUtf8("font: 75 8pt \"MS Shell Dlg 2\";"));

        gridLayout_10->addWidget(labelCor2_2, 0, 2, 1, 1);

        qvtkWidgetCor2 = new QVTKWidget(frameCoronalInf);
        qvtkWidgetCor2->setObjectName(QString::fromUtf8("qvtkWidgetCor2"));

        gridLayout_10->addWidget(qvtkWidgetCor2, 1, 0, 1, 3);

        labelCor2_3 = new QLabel(frameCoronalInf);
        labelCor2_3->setObjectName(QString::fromUtf8("labelCor2_3"));

        gridLayout_10->addWidget(labelCor2_3, 2, 2, 1, 1);

        labelCor2_1 = new QLabel(frameCoronalInf);
        labelCor2_1->setObjectName(QString::fromUtf8("labelCor2_1"));
        labelCor2_1->setMaximumSize(QSize(16777215, 14));

        gridLayout_10->addWidget(labelCor2_1, 0, 0, 1, 1);

        sliderCor2 = new QSlider(frameCoronalInf);
        sliderCor2->setObjectName(QString::fromUtf8("sliderCor2"));
        sliderCor2->setOrientation(Qt::Horizontal);

        gridLayout_10->addWidget(sliderCor2, 2, 0, 1, 1);


        gridLayout_12->addWidget(frameCoronalInf, 0, 2, 1, 1);

        progressBar = new QProgressBar(frameInf);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximumSize(QSize(200, 10));
        progressBar->setValue(24);

        gridLayout_12->addWidget(progressBar, 1, 0, 1, 1);


        gridLayout->addWidget(frameInf, 1, 0, 1, 1);

        AortaFlowApp->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(AortaFlowApp);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        AortaFlowApp->setStatusBar(statusbar);
        toolBar = new QToolBar(AortaFlowApp);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        AortaFlowApp->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidget_Recent_studies = new QDockWidget(AortaFlowApp);
        dockWidget_Recent_studies->setObjectName(QString::fromUtf8("dockWidget_Recent_studies"));
        dockWidget_Recent_studies->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidget_Recent_studies->sizePolicy().hasHeightForWidth());
        dockWidget_Recent_studies->setSizePolicy(sizePolicy);
        dockWidget_Recent_studies->setMinimumSize(QSize(194, 142));
        dockWidget_Recent_studies->setAutoFillBackground(true);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        dockWidget_Recent_studies->setWidget(dockWidgetContents);
        AortaFlowApp->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_Recent_studies);
        menubar = new QMenuBar(AortaFlowApp);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 832, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuWindow = new QMenu(menuView);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        AortaFlowApp->setMenuBar(menubar);
        dockWidget_Properties_window = new QDockWidget(AortaFlowApp);
        dockWidget_Properties_window->setObjectName(QString::fromUtf8("dockWidget_Properties_window"));
        dockWidget_Properties_window->setMinimumSize(QSize(207, 207));
        dockWidget_Properties_window->setAcceptDrops(false);
        dockWidget_Properties_window->setAutoFillBackground(true);
        dockWidget_Properties_window->setFloating(false);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        formLayout = new QFormLayout(dockWidgetContents_2);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        labelTimeStep = new QLabel(dockWidgetContents_2);
        labelTimeStep->setObjectName(QString::fromUtf8("labelTimeStep"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelTimeStep);

        spinBox = new QSpinBox(dockWidgetContents_2);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMaximumSize(QSize(60, 16777215));

        formLayout->setWidget(1, QFormLayout::FieldRole, spinBox);

        labelVelocityComponent = new QLabel(dockWidgetContents_2);
        labelVelocityComponent->setObjectName(QString::fromUtf8("labelVelocityComponent"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelVelocityComponent);

        labelSagSlice = new QLabel(dockWidgetContents_2);
        labelSagSlice->setObjectName(QString::fromUtf8("labelSagSlice"));

        formLayout->setWidget(4, QFormLayout::LabelRole, labelSagSlice);

        lineEdit = new QLineEdit(dockWidgetContents_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMaximumSize(QSize(60, 16777215));

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit);

        labelAxiSlice = new QLabel(dockWidgetContents_2);
        labelAxiSlice->setObjectName(QString::fromUtf8("labelAxiSlice"));

        formLayout->setWidget(5, QFormLayout::LabelRole, labelAxiSlice);

        lineEdit_2 = new QLineEdit(dockWidgetContents_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setMaximumSize(QSize(60, 16777215));

        formLayout->setWidget(5, QFormLayout::FieldRole, lineEdit_2);

        labelCorSlice = new QLabel(dockWidgetContents_2);
        labelCorSlice->setObjectName(QString::fromUtf8("labelCorSlice"));

        formLayout->setWidget(6, QFormLayout::LabelRole, labelCorSlice);

        lineEdit_3 = new QLineEdit(dockWidgetContents_2);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setMaximumSize(QSize(60, 16777215));

        formLayout->setWidget(6, QFormLayout::FieldRole, lineEdit_3);

        spinBox_2 = new QSpinBox(dockWidgetContents_2);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMinimumSize(QSize(0, 0));
        spinBox_2->setMaximumSize(QSize(60, 16777215));

        formLayout->setWidget(2, QFormLayout::FieldRole, spinBox_2);

        dockWidget_Properties_window->setWidget(dockWidgetContents_2);
        AortaFlowApp->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_Properties_window);

        toolBar->addAction(actionQuit);
        toolBar->addSeparator();
        toolBar->addAction(actionLoad_DICOM_series);
        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionLoad_DICOM_series);
        menuFile->addSeparator();
        menuFile->addAction(actionClose_current_serie);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuView->addAction(actionSuperior_frame);
        menuView->addAction(actionInferior_frame);
        menuView->addAction(action3D_View);
        menuView->addSeparator();
        menuView->addAction(actionSyncronize_both_frames);
        menuView->addSeparator();
        menuView->addAction(menuWindow->menuAction());
        menuWindow->addAction(actionFull_screen);
        menuWindow->addAction(actionNormal);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_Qt);

        retranslateUi(AortaFlowApp);
        QObject::connect(actionQuit, SIGNAL(triggered()), AortaFlowApp, SLOT(close()));
        QObject::connect(actionRecent_studies, SIGNAL(toggled(bool)), dockWidget_Recent_studies, SLOT(setVisible(bool)));
        QObject::connect(actionProperties_window, SIGNAL(toggled(bool)), dockWidget_Properties_window, SLOT(setVisible(bool)));
        QObject::connect(actionSuperior_frame, SIGNAL(toggled(bool)), frameSup, SLOT(setVisible(bool)));
        QObject::connect(actionInferior_frame, SIGNAL(toggled(bool)), frameInf, SLOT(setVisible(bool)));
        QObject::connect(actionFull_screen, SIGNAL(triggered()), AortaFlowApp, SLOT(showFullScreen()));
        QObject::connect(actionNormal, SIGNAL(triggered()), AortaFlowApp, SLOT(showNormal()));

        QMetaObject::connectSlotsByName(AortaFlowApp);
    } // setupUi

    void retranslateUi(QMainWindow *AortaFlowApp)
    {
        AortaFlowApp->setWindowTitle(QApplication::translate("AortaFlowApp", "Aorta Flow Application", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("AortaFlowApp", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("AortaFlowApp", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionRecent_studies->setText(QApplication::translate("AortaFlowApp", "Recent studies", 0, QApplication::UnicodeUTF8));
        actionSuperior_frame->setText(QApplication::translate("AortaFlowApp", "Magnitude frame (superior)", 0, QApplication::UnicodeUTF8));
        actionInferior_frame->setText(QApplication::translate("AortaFlowApp", "Flow frame (inferior)", 0, QApplication::UnicodeUTF8));
        actionProperties_window->setText(QApplication::translate("AortaFlowApp", "Properties window", 0, QApplication::UnicodeUTF8));
        actionLoad_DICOM_series->setText(QApplication::translate("AortaFlowApp", "Load DICOM series", 0, QApplication::UnicodeUTF8));
        actionLoad_DICOM_vectorial_serie->setText(QApplication::translate("AortaFlowApp", "Load DICOM vectorial serie", 0, QApplication::UnicodeUTF8));
        actionSyncronize_both_frames->setText(QApplication::translate("AortaFlowApp", "Syncronize both frames", 0, QApplication::UnicodeUTF8));
        actionFull_screen->setText(QApplication::translate("AortaFlowApp", "Full screen", 0, QApplication::UnicodeUTF8));
        actionFull_screen->setShortcut(QApplication::translate("AortaFlowApp", "F11", 0, QApplication::UnicodeUTF8));
        actionNormal->setText(QApplication::translate("AortaFlowApp", "Normal", 0, QApplication::UnicodeUTF8));
        actionNormal->setShortcut(QApplication::translate("AortaFlowApp", "F10", 0, QApplication::UnicodeUTF8));
        action3D_View->setText(QApplication::translate("AortaFlowApp", "3D View", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("AortaFlowApp", "About...", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("AortaFlowApp", "About Qt", 0, QApplication::UnicodeUTF8));
        actionClose_current_serie->setText(QApplication::translate("AortaFlowApp", "Close current serie", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        frameSagitalSup->setToolTip(QApplication::translate("AortaFlowApp", "This visor show the sagital plane of the magnitude images", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        frameSagitalSup->setWhatsThis(QApplication::translate("AortaFlowApp", "Este es el visor...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        labelSag1_2->setToolTip(QApplication::translate("AortaFlowApp", "Total number of slices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelSag1_2->setText(QApplication::translate("AortaFlowApp", "Slices:         ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        labelSag1_1->setToolTip(QApplication::translate("AortaFlowApp", "Slices plane orientation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelSag1_1->setText(QApplication::translate("AortaFlowApp", "Sagital(XY)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        labelSag1_3->setToolTip(QApplication::translate("AortaFlowApp", "Number of the current slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelSag1_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        frameAxialSup->setToolTip(QApplication::translate("AortaFlowApp", "This visor show the axial plane of the magnitude images", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        frameAxialSup->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        labelAxi1_2->setToolTip(QApplication::translate("AortaFlowApp", "Total number of slices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelAxi1_2->setText(QApplication::translate("AortaFlowApp", "Slices:         ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        labelAxi1_3->setToolTip(QApplication::translate("AortaFlowApp", "Number of the current slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelAxi1_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        labelAxi1_1->setToolTip(QApplication::translate("AortaFlowApp", "Slices plane orientation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelAxi1_1->setText(QApplication::translate("AortaFlowApp", "Axial(ZX)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        frameCoronalSup->setToolTip(QApplication::translate("AortaFlowApp", "This visor show the coronal plane of the magnitude images", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        labelCor1_2->setToolTip(QApplication::translate("AortaFlowApp", "Total number of slices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelCor1_2->setText(QApplication::translate("AortaFlowApp", "Slices:         ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        labelCor1_3->setToolTip(QApplication::translate("AortaFlowApp", "Number of the current slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelCor1_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        labelCor1_1->setToolTip(QApplication::translate("AortaFlowApp", "Slices plane orientation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelCor1_1->setText(QApplication::translate("AortaFlowApp", "Coronal(YZ)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        frameSagitalInf->setToolTip(QApplication::translate("AortaFlowApp", "This visor show the sagital plane of the flow images", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        labelSag2_2->setToolTip(QApplication::translate("AortaFlowApp", "Total number of slices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelSag2_2->setText(QApplication::translate("AortaFlowApp", "Slices:         ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        labelSag2_3->setToolTip(QApplication::translate("AortaFlowApp", "Number of the current slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelSag2_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        labelSag2_1->setToolTip(QApplication::translate("AortaFlowApp", "Slices plane orientation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelSag2_1->setText(QApplication::translate("AortaFlowApp", "Sagital(XY)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        frameAxialInf->setToolTip(QApplication::translate("AortaFlowApp", "This visor show the axial plane of the flow images", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        frameAxialInf->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        labelAxi2_2->setToolTip(QApplication::translate("AortaFlowApp", "Total number of slices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelAxi2_2->setText(QApplication::translate("AortaFlowApp", "Slices:         ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        labelAxi2_3->setToolTip(QApplication::translate("AortaFlowApp", "Number of the current slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelAxi2_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        labelAxi2_1->setToolTip(QApplication::translate("AortaFlowApp", "Slices plane orientation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelAxi2_1->setText(QApplication::translate("AortaFlowApp", "Axial(ZX)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        frameCoronalInf->setToolTip(QApplication::translate("AortaFlowApp", "This visor show the coronal plane of the flow images", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        labelCor2_2->setToolTip(QApplication::translate("AortaFlowApp", "Total number of slices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelCor2_2->setText(QApplication::translate("AortaFlowApp", "Slices:         ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        labelCor2_3->setToolTip(QApplication::translate("AortaFlowApp", "Number of the current slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelCor2_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        labelCor2_1->setToolTip(QApplication::translate("AortaFlowApp", "Slices plane orientation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelCor2_1->setText(QApplication::translate("AortaFlowApp", "Coronal(YZ)", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("AortaFlowApp", "toolBar", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        dockWidget_Recent_studies->setStatusTip(QApplication::translate("AortaFlowApp", "Recent studies used in the application", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        dockWidget_Recent_studies->setWindowTitle(QApplication::translate("AortaFlowApp", "  Recent studies", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("AortaFlowApp", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("AortaFlowApp", "View", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("AortaFlowApp", "Window", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("AortaFlowApp", "Help", 0, QApplication::UnicodeUTF8));
        dockWidget_Properties_window->setWindowTitle(QApplication::translate("AortaFlowApp", "  Properties window", 0, QApplication::UnicodeUTF8));
        labelTimeStep->setText(QApplication::translate("AortaFlowApp", "Time step", 0, QApplication::UnicodeUTF8));
        labelVelocityComponent->setText(QApplication::translate("AortaFlowApp", "Velocity component", 0, QApplication::UnicodeUTF8));
        labelSagSlice->setText(QApplication::translate("AortaFlowApp", "Sagital slices", 0, QApplication::UnicodeUTF8));
        labelAxiSlice->setText(QApplication::translate("AortaFlowApp", "Axial slices", 0, QApplication::UnicodeUTF8));
        labelCorSlice->setText(QApplication::translate("AortaFlowApp", "Coronal slices", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AortaFlowApp: public Ui_AortaFlowApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AORTAFLOWAPP_H
