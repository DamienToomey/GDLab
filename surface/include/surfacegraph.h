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

#include <QtWidgets/QSlider>
#include <QtWidgets/QLineEdit>
#include <QJSEngine>
#include <QJSValue>

using namespace QtDataVisualization;
using namespace std;

class MainWindow; // forward declaration because of circular dependency between MainWindow and SurfaceGraph (https://stackoverflow.com/questions/625799/resolve-build-errors-due-to-circular-dependency-amongst-classes)

class SurfaceGraph : public QObject
{
    Q_OBJECT

public:
    explicit SurfaceGraph(MainWindow *mainWindow);
    ~SurfaceGraph();
    QString executeSystemCommand(QString cmd);
    QSurface3DSeries* series();
    QString costFunction();
    QString dfdx();
    QString dfdz();
    QSurfaceDataProxy* proxy();
    QJSValue costFunctionEngine();
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
    void setCameraPreset();
    void resetCamera();
    void resetRange();
    void drawModel(QString arithmeticExpression);
    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void fillProxy(QString function);
    void computeGradient();
    void computeHessian();
    void setGradientIsComputed(bool gradientIsComputed);
    void setHessianIsComputed(bool hessianIsComputed);
    float originalXRotation();
    float originalYRotation();
    float originalZoomLevel();
    float xRotation();
    float yRotation();
    float zoomLevel();
    int cameraPreset();
    bool gradientIsComputed();
    bool hessianIsComputed();
    QJSValue initializeJSEngine(QString module, QString property);
    float evaluateFunction(QString function, float x, float z);

public Q_SLOTS:
    Q3DSurface* graph();
    void setTheme(int theme);
    void setCostFunction(int function);
    void setSelectionMode(int selectionMode);
    void setColormap(int colormap);
    void setSurface(int surface);

private:
    QJSEngine m_JSEngine;
    Q3DSurface *m_graph;
    QSurfaceDataProxy *m_proxy;
    QSurface3DSeries *m_series;
    QJSValue m_derivativeFunction;
    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;
    map<QString, QString> m_functionToArithmeticExpression;
    map<QString, QJSValue> m_functionToEvaluateFunction;
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
    bool m_costFunctionIsValid;
    bool m_gradientIsComputed;
    bool m_hessianIsComputed;
    MainWindow *m_mainWindow;

    QString preprocessArithmeticExpression(QString arithmeticExpression);
    QString mathjsPowerSymbolToJavascriptPowerSymbol(QString arithmeticExpression);
    QString javascriptPowerSymbolToMathjsPowerSymbol(QString arithmeticExpression);
    void computePartialDerivative(QString function, QString derivative, QString variable);
    QString cleanArithmeticExpression(QString arithmeticExpression);
    void setLineEditText(QLineEdit *lineEdit, QString text);
    void customizeAxes();
    void setPointIsSelected(bool pointIsSelected);
};

#endif // SURFACEGRAPH_H
