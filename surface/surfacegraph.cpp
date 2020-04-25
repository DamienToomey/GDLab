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

SurfaceGraph::SurfaceGraph(MainWindow *mainWindow)
    : m_originalXRotation(0.0f),
      m_originalYRotation(0.0f),
      m_originalZoomLevel(100.0f),
      m_cameraPreset(Q3DCamera::CameraPresetFrontLow),
      m_mainWindow(mainWindow)
{
    m_graph = mainWindow->graph();
    m_xRotation = m_originalXRotation;
    m_yRotation = m_originalYRotation;
    m_zoomLevel = m_originalZoomLevel;

    customizeAxes();

    m_proxy = new QSurfaceDataProxy();
    m_series = new QSurface3DSeries(m_proxy);
    m_series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_series->setFlatShadingEnabled(true);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
}

void SurfaceGraph::customizeAxes()
{
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
    m_graph->axisZ()->setRange(sampleMin, sampleMax);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::fillProxy(QJSValue costFunctionEngine)
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
            float y = costFunctionEngine.call(args).toNumber();
            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }
    m_proxy->resetArray(dataArray);
}

QString SurfaceGraph::formatArithmeticExpression(QString arithmeticExpression)
{
    return arithmeticExpression.toLower().simplified().remove(' ');
}

QString SurfaceGraph::preprocessArithmeticExpression(QString arithmeticExpression)
{
    arithmeticExpression = formatArithmeticExpression(arithmeticExpression);

    QRegExp rx("(\\b(?![xz])[a-z]+[0-9]*(?![\\(])\\b)");
    /**
    =================================== Regular expression explanation ===================================
    \\b : work on full words, not on substrings
    (?![xz])[a-z] : all the letters between a and z without x and z
    (?![\\(]) : do not take into account strings that finish with a parenthesis (e.g tan(x), sin(h), ...)
    [0-9]* : for math functions that have numbers in their name (e.g log10, log2, ...)
    ======================================================================================================
    **/
    rx.setMinimal(true);
    int s = -1;
    while((s = rx.indexIn(arithmeticExpression, s+1))>=0){
      arithmeticExpression.replace(s, rx.cap(0).length(), QString("Math.")+rx.cap(1).toUpper());
      s+= rx.cap(1).length();
    }

    arithmeticExpression.replace(QRegExp("([a-z]+[0-9]*\\()"), "Math.\\1");
    arithmeticExpression.replace("(-", "(-1*"); // exp(-x**2) does not work so I replace it with exp(-1*x**2)
    return arithmeticExpression;
}

QString SurfaceGraph::mathjsPowerSymbolToJavascriptPowerSymbol(QString arithmeticExpression)
{
    return arithmeticExpression.replace("^", "**");
}

QString SurfaceGraph::javascriptPowerSymbolToMathjsPowerSymbol(QString arithmeticExpression)
{
    return arithmeticExpression.replace("**", "^");
}

QString SurfaceGraph::computePartialDerivative(char variable)
{
   QString dfdx = executeSystemCommand(QString("mathjs \"derivative('%1', '%2')\"").arg(
                                       javascriptPowerSymbolToMathjsPowerSymbol(m_rawCostFunction),
                                       QString(variable)));
   dfdx = dfdx.trimmed();
   dfdx = mathjsPowerSymbolToJavascriptPowerSymbol(dfdx);
   dfdx = preprocessArithmeticExpression(dfdx);
   return dfdx;
}

void SurfaceGraph::setLineEditText(QLineEdit *lineEdit, QString text)
{
    lineEdit->setText(text);
    lineEdit->setCursorPosition(0);
}

void SurfaceGraph::computePartialDerivatives()
{
    if (m_costFunctionIsValid) {
        m_dfdx = computePartialDerivative('x');
        setLineEditText(m_mainWindow->dfdxLineEdit(), m_dfdx);
        m_dfdz = computePartialDerivative('z');
        setLineEditText(m_mainWindow->dfdzLineEdit(), m_dfdz);
        setPartialDerivarivesAreComputed(true);
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
        while (!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL) {
                data.append(buffer);
            }
        }
        pclose(stream);
    }
    return QString::fromStdString(data);
}

void SurfaceGraph::drawModel(QString arithmeticExpression)
{
    m_graph->removeCustomItems();
    m_costFunctionIsValid = true;
    setPartialDerivarivesAreComputed(false);
    m_rawCostFunction = formatArithmeticExpression(arithmeticExpression);
    m_costFunction = preprocessArithmeticExpression(arithmeticExpression);
    QJSValue costFunctionEngine = m_arithmeticEngine.evaluate(
                QString("(function(x, z) { return %1 ; })").arg(m_costFunction));

    if (costFunctionEngine.isError()) {
        qDebug() << "The expression is not valid";
        m_costFunctionIsValid = false;
    }
    else {
        m_graph->axisY()->setAutoAdjustRange(true);
        fillProxy(costFunctionEngine);
        m_graph->addSeries(m_series);
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
            break;
    }

    m_mainWindow->cameraPOVButton()->setText(QString("POV (%1)").arg(cameraPreset()));
    m_mainWindow->updateSliders();
    m_cameraPreset++;
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

//void SurfaceGraph::setCameraPreset()
//{
//    m_cameraPreset++;
//}

void SurfaceGraph::changeCostFunction(int function)
{
    QString arithmeticExpression;

    switch (function) {
        case MainWindow::InclinedTacoShell: {
            arithmeticExpression = "500*x*x+500*z";
            break;
        }
        case MainWindow::SqrtSin: {
            QString R = "(sqrt(z * z + x * x) + 0.01)";
            arithmeticExpression = QString("(sin(%1) / %1 + 0.24) * 1.61").arg(R);
            break;
        }
        case MainWindow::Saddle: {
            arithmeticExpression = "x * x - z * z";
            break;
        }
        case MainWindow::NonConvex: {
            arithmeticExpression = "3*exp(-(z+1)**2-x**2)*(x-1)**2-exp(-(x+1)**2-z**2)/3+exp(-x**2-z**2)*(10*x**3-2*x+10*z**5)"; //
            // Function found here : https://fr.mathworks.com/help/symbolic/graphics.html
            break;
        }
        default:
            break;
    }
    setLineEditText(m_mainWindow->fLineEdit(), arithmeticExpression);
    setLineEditText(m_mainWindow->dfdxLineEdit(), "");
    setLineEditText(m_mainWindow->dfdzLineEdit(), "");
    drawModel(arithmeticExpression);
}

void SurfaceGraph::changeSelectionMode(int selectionMode)
{

    switch (selectionMode) {
        case MainWindow::HideSelection: {
            toggleModeNone();
            m_mainWindow->setPointIsSelected(false);
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
            m_series->setDrawMode(QSurface3DSeries::DrawWireframe);
            break;
        }
        case 1: {
            m_series->setDrawMode(QSurface3DSeries::DrawSurface);
            break;
        }
        case 2: {
            m_series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
            break;
        }
        default:
            break;
    }
}

QSurface3DSeries* SurfaceGraph::series()
{
    return m_series;
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

void SurfaceGraph::setPartialDerivarivesAreComputed(bool partialDerivarivesAreComputed)
{
    m_partialDerivarivesAreComputed = partialDerivarivesAreComputed;
}

bool SurfaceGraph::partialDerivarivesAreComputed()
{
    return m_partialDerivarivesAreComputed;
}
