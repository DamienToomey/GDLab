#include "rmsprop.h"

RMSProp::RMSProp()
    : GradientDescent()
{
}

float RMSProp::updateRule(float xHat, float dfdx, float lr, float decayRate, float& dxSquared) {
    dxSquared = decayRate * dxSquared + (1 - decayRate) * dfdx * dfdx;
    xHat -= lr * dfdx / (sqrt(dxSquared) + 1e-7);
    return xHat;
}

vector<QVector3D> RMSProp::run()
{
    int k = 0;

    float dxSquared = 0;
    float dzSquared = 0;

    // Stanford University | Lecture 7
    // "This decay rate is something like 0.9 or 0.99"
    //float decayRate = 0.9;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > m_tol) && (k <= (int)m_nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, m_lr, m_decayRate, dxSquared);
        m_zHat = updateRule(m_zHat, m_dfdz, m_lr, m_decayRate, dzSquared);

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}

QColor RMSProp::color()
{
    return Qt::red;
}


QString RMSProp::name()
{
    return "RMSProp";
}

QList<QString> RMSProp::hyperParameters() {
    return { "lr", "tol", "decayRate", "nIterMax" };
}
