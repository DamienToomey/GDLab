#include "gradientdescent/rmsprop.h"

RMSProp::RMSProp()
    : GradientDescent()
{
    m_hyperParameterToDefaultValue["lr"] = 1e-3;
    m_hyperParameterToDefaultValue["tol"] = 0.005;
    m_hyperParameterToDefaultValue["nIterMax"] = 10000;

    // Stanford University | Lecture 7
    // "This decay rate is something like 0.9 or 0.99"
    m_hyperParameterToDefaultValue["decayRate"] = 0.9;

    m_hyperParameterToValue = m_hyperParameterToDefaultValue;
}

float RMSProp::updateRule(float xHat, float dfdx, float lr, float decayRate, float& dxSquared) {
    dxSquared = decayRate * dxSquared + (1 - decayRate) * dfdx * dfdx;
    xHat -= lr * dfdx / (sqrt(dxSquared) + 1e-7);
    return xHat;
}

vector<QVector3D> RMSProp::run()
{
    int k = 0;

    float lr = m_hyperParameterToValue["lr"];
    float tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];
    float decayRate = m_hyperParameterToValue["decayRate"];

    float dxSquared = 0;
    float dzSquared = 0;

    time_t start = time(0);
    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k <= nIterMax)) {
        m_cost = evaluateF(m_xHat, m_zHat);
        m_dfdx = evaluateDfdx(m_xHat, m_zHat);
        m_dfdz = evaluateDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, lr, decayRate, dxSquared);
        m_zHat = updateRule(m_zHat, m_dfdz, lr, decayRate, dzSquared);

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

QColor RMSProp::color()
{
    return Qt::red;
}


QString RMSProp::name()
{
    return "RMSProp";
}
