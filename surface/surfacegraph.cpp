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

#include "surfacegraph.h"
#include "mainwindow.h"

using namespace QtDataVisualization;

const int sampleCountX = 50;
const int sampleCountZ = 50;
const float sampleMin = -8.0f;
const float sampleMax = 8.0f;

SurfaceGraph::SurfaceGraph(Q3DSurface *surface, MainWindow *mainWindow)
    : m_graph(surface),
      m_originalXRotation(0.0f),
      m_originalYRotation(0.0f),
      m_originalZoomLevel(100.0f),
      m_cameraPreset(Q3DCamera::CameraPresetFrontLow),
      m_mainWindow(mainWindow)
{
    m_xRotation = m_originalXRotation;
    m_yRotation = m_originalYRotation;
    m_zoomLevel = m_originalZoomLevel;

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_graph->axisX()->setTitle(QStringLiteral("X"));
    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitle(QStringLiteral("Y"));
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitle(QStringLiteral("Z"));
    m_graph->axisZ()->setTitleVisible(true);

    m_graph->axisX()->setLabelFormat("%.2f");
    m_graph->axisZ()->setLabelFormat("%.2f");
    m_graph->axisX()->setRange(sampleMin, sampleMax);
    //m_graph->axisY()->setRange(0.0f, 2.0f);
    m_graph->axisZ()->setRange(sampleMin, sampleMax);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);

    m_Proxy = new QSurfaceDataProxy();
    m_Series = new QSurface3DSeries(m_Proxy);
    m_Series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_Series->setFlatShadingEnabled(true);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    //fillProxy();
    //m_arithmeticEngine.installExtensions(QJSEngine::AllExtensions);
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::fillProxy(QJSValue arithmeticfunction)
{
    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(sampleCountZ);
    for (int i = 0 ; i < sampleCountZ ; i++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
        // Keep values within range bounds, since just adding step can cause minor drift due
        // to the rounding errors.
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        int index = 0;
        for (int j = 0; j < sampleCountX; j++) {
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            QJSValueList args;
            args << x << z;
            float y = arithmeticfunction.call(args).toNumber();
            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }

    m_Proxy->resetArray(dataArray);
}

QString SurfaceGraph::preprocessArithmeticExpression(QString arithmeticexpression)
{
    // TO DO : log(x) does not work

    // Example expression
    // arithmeticexpression = "sin(x)+2*z*sin(cos(z))+tan(2)+x+log10(z)+logit(z)+xxtan(2)+amath(xxsin(z))+pi+e";

    arithmeticexpression = arithmeticexpression.toLower().simplified().remove(' ');

    m_originalArithmeticexpression = arithmeticexpression;

    // The following line cannot do toUpper on \\1
    //arithmeticexpression.replace(QRegularExpression("(\\b(?![xz])[a-z]+[0-9]*(?![\\(])\\b)"), "Math.\\1");
    // https://www.qtcentre.org/threads/12178-QString-replace()-with-QRegExp-capture-modification

    QRegExp rx("(\\b(?![xz])[a-z]+[0-9]*(?![\\(])\\b)");
    // \\b : full word
    // (?![xz])[a-z] : all the letters between a and z without x and z
    // (?![\\(]) : do not take into account substrings that finish with a parentheses (i.e tan(x), sin(h), ...)
    // [0-9]* : for log10, ...
    rx.setMinimal(true);
    int s = -1;
    while((s = rx.indexIn(arithmeticexpression, s+1))>=0){
      arithmeticexpression.replace(s, rx.cap(0).length(), QString("Math.")+rx.cap(1).toUpper());
      s+= rx.cap(1).length();
    }

    arithmeticexpression.replace(QRegExp("([a-z]+[0-9]*\\()"), "Math.\\1");
    arithmeticexpression.replace("(-", "(-1*"); // exp(-x**2) does not work so I replace it with exp(-x**2)*

//    QJSEngine myEngine;
//    QJSValue module = myEngine.importModule("./damienalgebra.mjs"); //

//    if (module.isError())
//        qDebug()
//                << "Uncaught exception at line"
//                << module.property("lineNumber").toInt()
//                << module.property("fileName").toString()
//                << ":" << module.toString();

//    QJSValue sumFunction = module.property("somme"); //
//    QJSValueList args;
//    //args << "x^2+4" << "x";
//    args << 2 << 3;
//    QJSValue result = sumFunction.call(args);
//    qDebug() << "hello " << result.toString();
//    if (result.isError())
//        qDebug()
//                << "Uncaught exception at line"
//                << result.property("lineNumber").toInt()
//                << ":" << result.toString();

    qDebug() << arithmeticexpression;

    // Install Qt 5.14.2
    // https://wiki.qt.io/Install_Qt_5_on_Ubuntu
    // qt-opensource-linux-x64-5.14.2.run : http://download.qt.io/official_releases/qt/5.14/5.14.2/
    // Qt 5.14.2 -> Desktop gcc 64-bit -> Qt Data Visualization -> Developer and Designer Tools

    return arithmeticexpression;
}

void SurfaceGraph::computePartialDerivatives()
{
    if (m_isArithmeticExpressionValid) {
        QString temp = m_originalArithmeticexpression.replace("**", "^");
        m_dfdx = executeSystemCommand(QString("mathjs \"derivative('%1','x')\"").arg(temp));
        m_dfdz = executeSystemCommand(QString("mathjs \"derivative('%1','z')\"").arg(temp));
        m_dfdx = m_dfdx.replace("^", "**").trimmed();
        m_dfdz = m_dfdz.replace("^", "**").trimmed();
        m_mainWindow->dfdxLineEdit()->setText(m_dfdx);
        m_mainWindow->dfdxLineEdit()->setCursorPosition(0);
        m_mainWindow->dfdzLineEdit()->setText(m_dfdz);
        m_mainWindow->dfdzLineEdit()->setCursorPosition(0);
    }
}

QString SurfaceGraph::executeSystemCommand(QString cmd)
{
    string data;
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.toStdString().c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return QString::fromStdString(data);
}

void SurfaceGraph::drawModel(QString arithmeticexpression)
{
    m_isArithmeticExpressionValid = true;
    QJSValue arithmeticfunction = m_arithmeticEngine.evaluate(
                QString("(function(x, z) { return %1 ; })").arg(preprocessArithmeticExpression(arithmeticexpression))
                );

    if (arithmeticfunction.isError()) {
        qDebug() << "The expression is not valid";
        m_isArithmeticExpressionValid = false;
    }
    else {
        m_costFunction = preprocessArithmeticExpression(arithmeticexpression);
        m_graph->axisY()->setAutoAdjustRange(true);
        fillProxy(arithmeticfunction);
        m_graph->addSeries(m_Series);
        resetRange();
    }
}

void SurfaceGraph::resetRange()
{
    // Reset range sliders for Sqrt&Sin
    m_rangeMinX = sampleMin;
    m_rangeMinZ = sampleMin;
    m_stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
    m_stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
    m_axisMinSliderX->setMaximum(sampleCountX - 2);
    m_axisMinSliderX->setValue(0);
    m_axisMaxSliderX->setMaximum(sampleCountX - 1);
    m_axisMaxSliderX->setValue(sampleCountX - 1);
    m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
    m_axisMinSliderZ->setValue(0);
    m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
    m_axisMaxSliderZ->setValue(sampleCountZ - 1);
}

void SurfaceGraph::adjustXMin(int min)
{
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraph::adjustZMax(int max)
{
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

void SurfaceGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void SurfaceGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}

void SurfaceGraph::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void SurfaceGraph::resetColormap()
{
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleUniform);
}

void SurfaceGraph::setColormap(tinycolormap::ColormapType colormap)
{
    QLinearGradient gr;
    for (float i = 0; i<=1; i += 0.1) {
        const QColor color = tinycolormap::GetColor(i, colormap).ConvertToQColor();
        gr.setColorAt(i, color);
    }
    // https://www.qtcentre.org/threads/14307-How-to-get-the-specified-position-s-QColor-in-QLinearGradient
    // https://github.com/yuki-koyama/tinycolormap

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void SurfaceGraph::rotateX(int rotation)
{
    m_xRotation = rotation;
    m_graph->scene()->activeCamera()->setXRotation(m_xRotation);
}

void SurfaceGraph::rotateY(int rotation)
{
    m_yRotation = rotation;
    m_graph->scene()->activeCamera()->setYRotation(m_yRotation);
}

void SurfaceGraph::zoom(int zoomLevel)
{
    m_zoomLevel = zoomLevel;
    m_graph->scene()->activeCamera()->setZoomLevel(m_zoomLevel);
}

void SurfaceGraph::changePresetCamera()
{
    if (m_cameraPreset >= Q3DCamera::CameraPresetFrontBelow) {
        m_cameraPreset = Q3DCamera::CameraPresetFrontLow;
    }

    switch (m_cameraPreset) {
    case Q3DCamera::CameraPresetFrontLow: {
        m_xRotation = 0.0f;
        m_yRotation = 0.0f;
        break;
    }
    case Q3DCamera::CameraPresetFront: {
        m_xRotation = 0.0f;
        m_yRotation = 22.5f;
        break;
    }
    case Q3DCamera::CameraPresetFrontHigh: {
        m_xRotation = 0.0f;
        m_yRotation = 45.0f;
        break;
    }
    case Q3DCamera::CameraPresetLeftLow: {
        m_xRotation = 90.0f;
        m_yRotation = 0.0f;
        break;
    }
    case Q3DCamera::CameraPresetLeft: {
        m_xRotation = 90.0f;
        m_yRotation = 22.5f;
        break;
    }
    case Q3DCamera::CameraPresetLeftHigh: {
        m_xRotation = 90.0f;
        m_yRotation = 45.0f;
        break;
    }
    case Q3DCamera::CameraPresetRightLow: {
        m_xRotation = -90.0f;
        m_yRotation =0.0f;
        break;
    }
    case Q3DCamera::CameraPresetRight: {
        m_xRotation = -90.0f;
        m_yRotation = 22.5f;
        break;
    }
    case Q3DCamera::CameraPresetRightHigh: {
        m_xRotation = -90.0f;
        m_yRotation = 45.0f;
        break;
    }
    case Q3DCamera::CameraPresetBehindLow: {
        m_xRotation = 180.0f;
        m_yRotation = 0.0f;
        break;
    }
    case Q3DCamera::CameraPresetBehind: {
        m_xRotation = 180.0f;
        m_yRotation = 22.5f;
        break;
    }
    case Q3DCamera::CameraPresetBehindHigh: {
        m_xRotation = 180.0f;
        m_yRotation = 45.0f;
        break;
    }
    case Q3DCamera::CameraPresetIsometricLeft: {
        m_xRotation = 45.0f;
        m_yRotation = 22.5f;
        break;
    }
    case Q3DCamera::CameraPresetIsometricLeftHigh: {
        m_xRotation = 45.0f;
        m_yRotation = 45.0f;
        break;
    }
    case Q3DCamera::CameraPresetIsometricRight: {
        m_xRotation = -45.0f;
        m_yRotation = 22.5f;
        break;
    }
    case Q3DCamera::CameraPresetIsometricRightHigh: {
        m_xRotation = -45.0f;
        m_yRotation = 45.0f;
        break;
    }
    case Q3DCamera::CameraPresetDirectlyAbove: {
        m_xRotation = 0.0f;
        m_yRotation = 90.0f;
        break;
    }
    case Q3DCamera::CameraPresetDirectlyAboveCW45: {
        m_xRotation = -45.0f;
        m_yRotation = 90.0f;
        break;
    }
    case Q3DCamera::CameraPresetDirectlyAboveCCW45: {
        m_xRotation = 45.0f;
        m_yRotation = 90.0f;
        break;
    }
    default:
        m_cameraPreset = Q3DCamera::CameraPresetFrontLow;
        break;
    }
}

float SurfaceGraph::originalXRotation()
{
    return m_originalXRotation;
}

float SurfaceGraph::originalYRotation()
{
    return m_originalYRotation;
}

float SurfaceGraph::originalZoomLevel()
{
    return m_originalZoomLevel;
}

float SurfaceGraph::xRotation()
{
    return m_xRotation;
}

float SurfaceGraph::yRotation()
{
    return m_yRotation;
}

float SurfaceGraph::zoomLevel()
{
    return m_zoomLevel;
}

int SurfaceGraph::cameraPreset()
{
    return m_cameraPreset;
}

void SurfaceGraph::setCameraPreset()
{
    m_cameraPreset++;
}

void SurfaceGraph::changeFunction(int function)
{
    QString arithmeticexpression;

    switch (function) {
    case MainWindow::InclinedTacoShell: {
        arithmeticexpression = "500*x*x+500*z";
        break;
    }
    case MainWindow::SqrtSin: {
        QString R = "(sqrt(z * z + x * x) + 0.01)";
        arithmeticexpression = QString("(sin(%1) / %1 + 0.24) * 1.61").arg(R);
        break;
    }
    case MainWindow::Saddle: {
        arithmeticexpression = "x * x - z * z";
        break;
    }
    case MainWindow::NonConvex: {
        arithmeticexpression = "3*exp(-(z+1)**2-x**2)*(x-1)**2-exp(-(x+1)**2-z**2)/3+exp(-x**2-z**2)*(10*x**3-2*x+10*z**5)"; //
        // https://fr.mathworks.com/help/symbolic/graphics.html
        break;
    }
    default:
        break;
    }
    m_mainWindow->fLineEdit()->setText(arithmeticexpression);
    m_mainWindow->fLineEdit()->setCursorPosition(0);
    drawModel(arithmeticexpression);
}

void SurfaceGraph::changeSelectionMode(int selectionMode)
{

    switch (selectionMode) {
    case MainWindow::HideSelection: {
        toggleModeNone();
        break;
    }
    case MainWindow::ShowSelection: {
        toggleModeItem();
        break;
    }
    case MainWindow::RowSlice: {
        toggleModeSliceRow();
        break;
    }
    case MainWindow::ColumnSlice: {
        toggleModeSliceColumn();
        break;
    }
    default:
        break;
    }
}

void SurfaceGraph::changeColormap(int colormap)
{
    switch (colormap) {
    case 0: {
        resetColormap();
        break;
    }
    case 1: {
        setColormap(tinycolormap::ColormapType::Heat);
        break;
    }
    case 2: {
        setColormap(tinycolormap::ColormapType::Jet);
        break;
    }
    case 3: {
        setColormap(tinycolormap::ColormapType::Hot);
        break;
    }
    case 4: {
        setColormap(tinycolormap::ColormapType::Gray);
        break;
    }
    case 5: {
        setColormap(tinycolormap::ColormapType::Magma);
        break;
    }
    case 6: {
        setColormap(tinycolormap::ColormapType::Inferno);
        break;
    }
    case 7: {
        setColormap(tinycolormap::ColormapType::Plasma);
        break;
    }
    case 8: {
        setColormap(tinycolormap::ColormapType::Viridis);
        break;
    }
    case 9: {
        setColormap(tinycolormap::ColormapType::Cividis);
        break;
    }
    case 10: {
        setColormap(tinycolormap::ColormapType::Github);
        break;
    }
    default:
        break;
    }
}

void SurfaceGraph::changeSurface(int surface)
{
    switch (surface) {
    case 0: {
        m_Series->setDrawMode(QSurface3DSeries::DrawWireframe);
        break;
    }
    case 1: {
        m_Series->setDrawMode(QSurface3DSeries::DrawSurface);
        break;
    }
    case 2: {
        m_Series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        break;
    }
    default:
        break;
    }
}

QSurface3DSeries* SurfaceGraph::series()
{
    return m_Series;
}

QString SurfaceGraph::costFunction()
{
    return m_costFunction;
}

QString SurfaceGraph::dfdx()
{
    return m_dfdx;
}

QString SurfaceGraph::dfdz()
{
    return m_dfdz;
}

Q3DSurface* SurfaceGraph::graph()
{
    return m_graph;
}

//    QObject::connect(m_modeNoneRB, &QRadioButton::toggled,
//                     m_modifier, &SurfaceGraph::toggleModeNone);
//    QObject::connect(m_modeItemRB,  &QRadioButton::toggled,
//                     m_modifier, &SurfaceGraph::toggleModeItem);
//    QObject::connect(m_modeSliceRowRB,  &QRadioButton::toggled,
//                     m_modifier, &SurfaceGraph::toggleModeSliceRow);
//    QObject::connect(m_modeSliceColumnRB,  &QRadioButton::toggled,
//                     m_modifier, &SurfaceGraph::toggleModeSliceColumn);

// 3D Functions
// http://euler.rene-grothmann.de/Programs/03%20-%203D%20Graphics.html


// ################################################
// ##### Cannot see under the surface problem #####
// ################################################

// Problem caused in function updateScene

// When looking at the method updateScene in bars3drenderer.cpp and scatter3drenderer.cpp,
// there is a line that sets minYRotation :
// scene->activeCamera()->d_ptr->setMinYRotation(-90.0);
// whereas there no line like that for surface3drenderer so the minYRotation is blocked at 0 for Q3DSurface
// since Q3DCameraPrivate (in q3dcamera.cpp) is instanciated with m_minYRotation(0.0f)

// https://code.woboq.org/qt5/qtdatavis3d/src/datavisualization/engine/bars3drenderer.cpp.html
// https://code.woboq.org/qt5/qtdatavis3d/src/datavisualization/engine/scatter3drenderer.cpp.html
// https://code.woboq.org/qt5/qtdatavis3d/src/datavisualization/engine/surface3drenderer.cpp.html
// https://code.woboq.org/qt5/qtdatavis3d/src/datavisualization/engine/q3dcamera.cpp.html
