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

#include "surfacegraph.h"

#define TINYCOLORMAP_WITH_QT5
#include "tinycolormap.hpp"

class SurfaceGraph; // forward declaration because of circular dependency between MainWindow and SurfaceGraph (https://stackoverflow.com/questions/625799/resolve-build-errors-due-to-circular-dependency-amongst-classes)

class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow(Q3DSurface *graph, QWidget *container);
    QLineEdit* dfdxLineEdit();
    QLineEdit* dfdzLineEdit();
    QLineEdit* fLineEdit();
    enum Functions { InclinedTacoShell = 0, SqrtSin = 1, Saddle = 2, NonConvex = 3 };
    enum SelectionMode { HideSelection = 0, ShowSelection = 1, RowSlice = 2, ColumnSlice = 3 };

public Q_SLOTS:
    void resetCamera();
    void updateSliders();
    void updateXRotationSlider(float rotation);
    void updateYRotationSlider(float rotation);
    void updateZoomLevelSlider(float rotation);
    void setSelectedPoint(QPoint selectedPoint);
    QVector3D selectedPoint();
    void runGradientDescent();

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
    QComboBox *m_selectionModeList;
    QComboBox *m_colormapList;
    QComboBox *m_surfaceList;
    QVector3D m_selectedPoint;
    QPushButton *m_runGDButton;
};

#endif // MAINWINDOW_H
