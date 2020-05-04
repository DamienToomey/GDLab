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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtDataVisualization/QCustom3DItem>
#include <QDebug>

#include "surfacegraph.h"

#include "resultwindow.h"

#include "gradientdescent/vanillagradientdescent.h"
#include "gradientdescent/gradientdescentwithmomentum.h"
#include "gradientdescent/nesterovmomentum.h"
#include "gradientdescent/adagrad.h"
#include "gradientdescent/rmsprop.h"
#include "gradientdescent/adam.h"
#include "gradientdescent/newtonregularized.h"

#define TINYCOLORMAP_WITH_QT5
#include "tinycolormap.hpp"

using namespace tinycolormap;

class SurfaceGraph; // forward declaration because of circular dependency between MainWindow and SurfaceGraph (https://stackoverflow.com/questions/625799/resolve-build-errors-due-to-circular-dependency-amongst-classes)

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(Q3DSurface *graph, QWidget *container);
    ~MainWindow();
    enum Functions { InclinedTacoShell = 0, SqrtSin = 1, NarrowSaddle = 2, NonConvex = 3, WideSaddle = 4 };
    enum Views { Surface3D = 0, RowSlice = 1, ColumnSlice = 2 };
    QLineEdit* dfdxLineEdit();
    QLineEdit* dfdzLineEdit();
    QLineEdit* costFunctionLineEdit();
    QVector3D selectedPoint();
    Q3DSurface* graph();
    QPushButton* cameraPOVButton();
    void togglePoints(GradientDescent *gradientDescentMethod, bool showCurve);
    void plotPoints(GradientDescent *gradientDescentMethod);
    void initializeInitializationPointRandomly();
    void setPointIsSelected(bool pointIsSelected);
    bool pointIsOnSurface(QPoint selectedPoint);
    bool pointIsSelected();
    SurfaceGraph* modifier();
    vector<GradientDescent*> visibleCurvesMemory();
    map<int, QLinearGradient> intToLinearGradient();
    map<QString, QCheckBox*> gradientDescentMethodToCheckBox();
    map<QString, GradientDescent*> gradientDescentMethodToGradientDescent();
    map<QString, QLineEdit*> functionToLineEdit();
    QLineEdit* functionToLineEdit(QString function);
    QDoubleSpinBox* ySpinBox();

public Q_SLOTS:
    void resetCamera();
    void updateSliders();
    void updateXRotationSlider(float rotation);
    void updateYRotationSlider(float rotation);
    void updateZoomLevelSlider(float rotation);
    void setSelectedPoint(QPoint selectedPoint);
    void runGradientDescent();
    void toggleCurve(bool checked);
    void toggleCurves(bool);
    void resetValues();
    void toggleCheckboxes(bool);
    void openResultWindow();

private:
    QWidget *m_container;
    QWidget *m_widget;
    Q3DSurface *m_graph;
    SurfaceGraph *m_modifier;
    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
    QSlider *m_rotationSliderX;
    QSlider *m_rotationSliderY;
    QSlider *m_zoomSlider;
    QComboBox *m_themeList;
    QComboBox *m_costFunctionList;
    QComboBox *m_viewList;
    QComboBox *m_colormapList;
    QComboBox *m_surfaceList;
    QComboBox *m_gradientDescentCurveList;
    QPushButton *m_resetCameraButton;
    QPushButton *m_cameraPOVButton;
    QPushButton *m_computeGradientButton;
    QPushButton *m_computeHessianButton;
    QPushButton *m_runGradientDescentButton;
    QPushButton *m_toggleCurvesButton;
    QPushButton *m_resetValuesButton;
    QPushButton *m_toggleCheckboxesButton;
    QPushButton *m_resultButton;
    QDoubleSpinBox *m_xSpinBox;
    QDoubleSpinBox *m_ySpinBox;
    QDoubleSpinBox *m_zSpinBox;
    QVector3D m_selectedPoint;
    QCustom3DItem *m_previousItem = new QCustom3DItem();
    bool m_pointIsSelected = false;

    map<QString, QPushButton*> m_gradientDescentMethodToPushButton;
    map<QString, QCheckBox*> m_gradientDescentMethodToCheckBox;
    map<QString, QDoubleSpinBox*> m_keyToSpinBox;
    QString key(QString gradientDescentMethod, QString hyperParameter);
    map<QString, GradientDescent*> m_gradientDescentMethodToGradientDescent;
    map<QString, int> m_gradientDescentMethodToInt;
    map<int, QString> m_intToGradientDescentMethod;
    map<int, QLinearGradient> m_intToLinearGradient;
    map<QString, QLineEdit*> m_functionToLineEdit;

    QScrollArea* initializeScrollArea(QScrollArea *scrollArea);
    void initializeLeftVLayout(QVBoxLayout *leftVLayout);
    void initializeRightVLayout(QVBoxLayout *rightVLayout);
};

#endif // MAINWINDOW_H
