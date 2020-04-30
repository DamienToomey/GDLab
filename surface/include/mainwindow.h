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
#include <QDoubleSpinBox>

#include "surfacegraph.h"

#include "gradientdescent/vanillagradientdescent.h"
#include "gradientdescent/gradientdescentwithmomentum.h"
#include "gradientdescent/nesterovmomentum.h"
#include "gradientdescent/adagrad.h"
#include "gradientdescent/rmsprop.h"
#include "gradientdescent/adam.h"

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
    enum Functions { InclinedTacoShell = 0, SqrtSin = 1, NarrowSaddle = 2, NonConvex = 3, WideSaddle = 4 };
    enum Views { Surface3D = 0, RowSlice = 1, ColumnSlice = 2 };
    enum GradientDescentMethods { _VanillaGradientDescent = 0,
                                  _GradientDescentWithMomentum = 1,
                                  _NesterovMomentum = 2,
                                  _AdaGrad = 3,
                                  _RMSProp = 4,
                                  _Adam = 5};
    QVector3D selectedPoint();
    bool pointIsOnSurface(QPoint selectedPoint);
    void setPointIsSelected(bool pointIsSelected);
    Q3DSurface* graph();
    SurfaceGraph* modifier();
    bool pointIsSelected();
    QPushButton* cameraPOVButton();
    void initializeInitializationPointRandomly();
    vector<GradientDescent*> visibleCurvesMemory();
    void togglePoints(GradientDescent *gradientDescentMethod, bool showCurve);
    void plotPoints(GradientDescent *gradientDescentMethod);

public Q_SLOTS:
    void resetCamera();
    void updateSliders();
    void updateXRotationSlider(float rotation);
    void updateYRotationSlider(float rotation);
    void updateZoomLevelSlider(float rotation);
    void setSelectedPoint(QPoint selectedPoint);
    void runGradientDescent();
    void toggleCurve(bool checked);
    void toggleCurves();

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
    QDoubleSpinBox *m_xSpinBox;
    QDoubleSpinBox *m_ySpinBox;
    QDoubleSpinBox *m_zSpinBox;
    QString key(MainWindow::GradientDescentMethods gradientDescentMethod, QString hyperParameter);
    void initializeLeftVLayout(QVBoxLayout *leftVLayout);
    void initializeRightVLayout(QVBoxLayout *rightVLayout);
    map<QString, QDoubleSpinBox*> m_keyToSpinBox;
    void setPredefinedValues(QDoubleSpinBox *spinBox, QString hyperParameter);
    map<MainWindow::GradientDescentMethods, QPushButton*> m_gradientDescentMethodToPushButton;
    map<MainWindow::GradientDescentMethods, QCheckBox*> m_gradientDescentMethodToCheckBox;
    QPushButton *m_toggleCurvesButton;
};

#endif // MAINWINDOW_H
