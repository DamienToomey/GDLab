#include "vanillagradientdescent.h"

VanillaGradientDescent::VanillaGradientDescent(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
    m_modifier = m_mainWindow->modifier();
    m_graph = m_mainWindow->graph();

    m_costFunctionEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->costFunction()));
    m_dfdxEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->dfdx()));
    m_dfdzEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->dfdz()));

    QVector3D initialPoint = mainWindow->selectedPoint();
    m_xHat = initialPoint.x(); // weight in Deep Learning
    m_zHat = initialPoint.z(); // weight in Deep Learning
    m_cost = computeCostFunction(m_xHat, m_zHat);
    m_dfdx = 1; // partial derivative (cf gradient in Deep Learning)
    m_dfdz = 1; // partial derivative (cf gradient in Deep Learning)
    m_nIterMax = 100000;
    m_lr = 1e-3; // learning rate
    m_tol = 0.005;
}

void VanillaGradientDescent::plotPoint()
{
    QImage color = QImage(2, 2, QImage::Format_RGB32);
    color.fill(Qt::blue);
    QVector3D positionOne = QVector3D(m_xHat, m_cost, m_zHat);
    QCustom3DItem *item = new QCustom3DItem(":/sphere/sphere", positionOne,
                                            QVector3D(0.02f, 0.02f, 0.02f),
                                            QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 0.0f, 0.0f),
                                            color);
    m_graph->addCustomItem(item);
}

float VanillaGradientDescent::run()
{
    int k = 0;
    plotPoint();
    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > m_tol) && (k <= m_nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);
        m_xHat = m_xHat-m_lr*m_dfdx;
        m_zHat = m_zHat-m_lr*m_dfdz;

        if (k % 10 == 0) {
            plotPoint();
        }

        k = k+1;
    }
    plotPoint();
    return k;
}

float VanillaGradientDescent::computeCostFunction(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_costFunctionEngine.call(args).toNumber();;
}

float VanillaGradientDescent::computeDfdx(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_dfdxEngine.call(args).toNumber();;
}

float VanillaGradientDescent::computeDfdz(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_dfdzEngine.call(args).toNumber();;
}
