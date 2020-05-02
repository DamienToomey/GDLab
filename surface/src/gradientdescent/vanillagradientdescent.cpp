#include "gradientdescent/vanillagradientdescent.h"

VanillaGradientDescent::VanillaGradientDescent()
    : GradientDescent()
{
    m_hyperParameterToDefaultValue["lr"] = 1e-3;
    m_hyperParameterToDefaultValue["tol"] = 0.005;
    m_hyperParameterToDefaultValue["nIterMax"] = 10000;

    m_hyperParameterToValue = m_hyperParameterToDefaultValue;
}

float VanillaGradientDescent::updateRule(float xHat, float dfdx, float lr) {
    xHat -= lr * dfdx;
    return xHat;
}

vector<QVector3D> VanillaGradientDescent::run()
{
    int k = 1;

    float lr = m_hyperParameterToValue["lr"];
    float tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];

    time_t start = time(0);
    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k <= nIterMax)) {
        m_cost = evaluateF(m_xHat, m_zHat);
        m_dfdx = evaluateDfdx(m_xHat, m_zHat);
        m_dfdz = evaluateDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, lr);
        m_zHat = updateRule(m_zHat, m_dfdz, lr);

        m_points.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    time_t end = time(0);

    m_statisticLabelToValue["Execution time"] = end - start;
    m_statisticLabelToValue["Last cost value"] = m_points.end()->y();
    m_statisticLabelToValue["Number of iterations"] = k;
    m_statisticLabelToValue["Reached nIterMax"] = (k >= nIterMax) ? 1 : 0;

    return m_points;
}

QColor VanillaGradientDescent::color()
{
    return Qt::black;
}


QString VanillaGradientDescent::name()
{
    return "Vanilla GD";
}
