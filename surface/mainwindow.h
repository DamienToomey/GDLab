#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QDebug>
#include <QtWidgets/QLineEdit>
#include <QMessageBox>

#include "surfacegraph.h"

#include "gradientdescent/vanillagradientdescent.h"
#include "gradientdescent/gradientdescentwithmomentum.h"
#include "gradientdescent/nesterovmomentum.h"

#define TINYCOLORMAP_WITH_QT5
#include "tinycolormap.hpp"

class SurfaceGraph; // forward declaration because of circular dependency between MainWindow and SurfaceGraph (https://stackoverflow.com/questions/625799/resolve-build-errors-due-to-circular-dependency-amongst-classes)

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(Q3DSurface *graph, QWidget *container);
    QLineEdit* dfdxLineEdit();
    QLineEdit* dfdzLineEdit();
    QLineEdit* fLineEdit();
    enum Functions { InclinedTacoShell = 0, SqrtSin = 1, Saddle = 2, NonConvex = 3 };
    enum Views { Surface3D = 0, RowSlice = 1, ColumnSlice = 2 };
    enum GradientDescentMethods { _VanillaGradientDescent = 0, _GradientDescentWithMomentum = 1, _NesterovMomentum = 2 };

public Q_SLOTS:
    void resetCamera();
    void updateSliders();
    void updateXRotationSlider(float rotation);
    void updateYRotationSlider(float rotation);
    void updateZoomLevelSlider(float rotation);
    void setSelectedPoint(QPoint selectedPoint);
    QVector3D selectedPoint();
    void runGradientDescent();
    Q3DSurface* graph();
    SurfaceGraph* modifier();
    bool pointIsOnSurface(QPoint selectedPoint);
    void setPointIsSelected(bool pointIsSelected);
    bool pointIsSelected();
    QPushButton* cameraPOVButton();
    void plotPoints(GradientDescent *gradientDescentMethod);
    void togglePoints(GradientDescent *gradientDescentMethod, bool showCurve);
    void toggleCurve(int curve);
    void toggleCurves(bool showCurve);
    bool atLeastOneCurveIsVisible(int n_visibleCurves);
    bool allCurvesWereVisibleBeforeHidingThem(vector<GradientDescent*> visibleCurvesMemory);
    void initializeInitializationPointRandomly();
    vector<GradientDescent*> visibleCurvesMemory();

private:
    Q3DSurface *m_graph;
    QWidget *m_container;
    QWidget *m_widget;
    SurfaceGraph *m_modifier;
    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
    QComboBox *m_themeList;
    QSlider *m_rotationSliderX;
    QSlider *m_rotationSliderY;
    QSlider *m_zoomSlider;
    QPushButton *m_resetCameraButton;
    QPushButton *m_cameraPOVButton;
    QLineEdit *m_fLineEdit;
    QLineEdit *m_dfdxLineEdit;
    QLineEdit *m_dfdzLineEdit;
    QPushButton *m_computePartialDerivativesButton;
    QComboBox *m_functionList;
    QComboBox *m_viewList;
    QComboBox *m_colormapList;
    QComboBox *m_surfaceList;
    QVector3D m_selectedPoint;
    QPushButton *m_runGradientDescentButton;
    bool m_pointIsSelected = false;
    QComboBox *m_gradientDescentCurveList;
    map<GradientDescentMethods, GradientDescent*> m_gdName2gdObject;
    vector<GradientDescent*> m_visibleCurvesMemory;
};

#endif // MAINWINDOW_H
