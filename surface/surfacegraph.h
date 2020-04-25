/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtCore/QPropertyAnimation>
#include <QJSEngine>
#include <QJSValue>
#include <QJSValueList>
#include <QString>
#include <string>
#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QCoreApplication>

#define TINYCOLORMAP_WITH_QT5
#include "tinycolormap.hpp"

using namespace QtDataVisualization;
using namespace std;

class MainWindow; // forward declaration because of circular dependency between MainWindow and SurfaceGraph (https://stackoverflow.com/questions/625799/resolve-build-errors-due-to-circular-dependency-amongst-classes)

class SurfaceGraph : public QObject
{
    Q_OBJECT

public:
    explicit SurfaceGraph(Q3DSurface *surface, MainWindow *mainWindow);
    ~SurfaceGraph();

    void enableSqrtSinModel(bool enable);

    void toggleModeNone() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice); }
    void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);

    void rotateX(int rotation);
    void rotateY(int rotation);
    void zoom(int zoomLevel);
    void changePresetCamera();
    void resetCamera();
    float originalXRotation();
    float originalYRotation();
    float originalZoomLevel();
    float xRotation();
    float yRotation();
    float zoomLevel();
    int cameraPreset();
    void setCameraPreset();
    void resetRange();
    void drawModel(QString arithmeticexpression);
    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void fillProxy(QJSValue arithmeticfunction);
    QString executeSystemCommand(QString cmd);
    void computePartialDerivatives();
    void resetColormap();
    void setColormap(tinycolormap::ColormapType colormap);
    QSurface3DSeries* series();
    QString costFunction();
    QString dfdx();
    QString dfdz();

public Q_SLOTS:
    void changeTheme(int theme);
    void changeFunction(int function);
    void changeSelectionMode(int selectionMode);
    void changeColormap(int colormap);
    void changeSurface(int surface);
    Q3DSurface* graph();
    QString preprocessArithmeticExpression(QString arithmeticexpression);

private:
    Q3DSurface *m_graph;
    QSurfaceDataProxy *m_Proxy;
    QSurface3DSeries *m_Series;

    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
    float m_rangeMinX;
    float m_rangeMinZ;
    float m_stepX;
    float m_stepZ;
    float m_xRotation;
    float m_yRotation;
    float m_zoomLevel;
    float m_originalXRotation;
    float m_originalYRotation;
    float m_originalZoomLevel;
    int m_cameraPreset;
    QJSEngine m_arithmeticEngine;
    QString m_originalArithmeticexpression;
    bool m_isArithmeticExpressionValid = false;
    QString m_dfdx;
    QString m_dfdz;
    MainWindow *m_mainWindow;
    QString m_costFunction;
};

#endif // SURFACEGRAPH_H
