#include "gradientdescent.h"

int GradientDescent::m_curveId = 0;

GradientDescent::GradientDescent()
{
    setRotation(m_curveId);
    GradientDescent::m_curveId++;
}

void GradientDescent::initialize(SurfaceGraph *modifier, QVector3D initializationPoint)
{
    m_modifier = modifier;
    m_costFunctionEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->costFunction()));
    m_dfdxEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->dfdx()));
    m_dfdzEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->dfdz()));

    m_pointsTable.clear();

    m_xHat = initializationPoint.x(); // weight in Deep Learning
    m_zHat = initializationPoint.z(); // weight in Deep Learning
    m_cost = initializationPoint.y(); // <=> computeCostFunction(m_xHat, m_zHat);
    m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

    // Initialize partial derivatives such as the norm of the gradient is greater
    // than the tolerance chosen in the while loop of the child class
    m_dfdx = 1; // partial derivative (gradient in Deep Learning)
    m_dfdz = 1; // partial derivative (gradient in Deep Learning)
}

float GradientDescent::computeCostFunction(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_costFunctionEngine.call(args).toNumber();
}

float GradientDescent::computeDfdx(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_dfdxEngine.call(args).toNumber();
}

float GradientDescent::computeDfdz(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_dfdzEngine.call(args).toNumber();
}

vector<QVector3D> GradientDescent::pointsTable()
{
    return m_pointsTable;
}

void GradientDescent::setRotation(int curveId)
{
    m_rotation = QQuaternion::fromAxisAndAngle(curveId, curveId, curveId, curveId);
}

QQuaternion GradientDescent::rotation()
{
    return m_rotation;
}
