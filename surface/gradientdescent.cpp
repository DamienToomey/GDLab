#include "gradientdescent.h"

GradientDescent::GradientDescent(MainWindow *mainWindow)
    : m_mainWindow(mainWindow)
{
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
    m_nIterMax = 10000; // 10000 is too much, so values on y axis are to high, thus crashing the program
    m_lr = 1e-3; // learning rate
    m_tol = 0.005;
}

void GradientDescent::plotPoint(int k)
{
    if (k % 1 == 0) {
        qDebug() << "k" << k;
        qDebug() << "m_xHat" << m_xHat;
        qDebug() << "m_yHat" << m_cost;
        qDebug() << "m_zHat" << m_zHat;
        QImage color = QImage(2, 2, QImage::Format_RGB32);
        color.fill(Qt::blue);
        QVector3D positionOne = QVector3D(m_xHat, m_cost, m_zHat);
        QCustom3DItem *item = new QCustom3DItem(":/sphere/sphere", positionOne,
                                                QVector3D(0.02f, 0.02f, 0.02f),
                                                QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 0.0f, 0.0f),
                                                color);
        m_graph->addCustomItem(item);
    }
}

float GradientDescent::computeCostFunction(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_costFunctionEngine.call(args).toNumber();;
}

float GradientDescent::computeDfdx(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_dfdxEngine.call(args).toNumber();;
}

float GradientDescent::computeDfdz(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_dfdzEngine.call(args).toNumber();;
}
