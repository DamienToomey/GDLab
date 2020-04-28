#include "adagrad.h"

AdaGrad::AdaGrad()
    : GradientDescent()
{
}

float AdaGrad::updateRule(float xHat, float dfdx, float lr, float& dxSquared)
{
    dxSquared += dfdx * dfdx;
    xHat -= lr * dfdx / (sqrt(dxSquared) + 1e-7);
    return xHat;
}

vector<QVector3D> AdaGrad::run(float lr, float tol, int nIterMax)
{
    int k = 1;

    float dxSquared = 0;
    float dzSquared = 0;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k <= nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, lr, dxSquared);
        m_zHat = updateRule(m_zHat, m_dfdz, lr, dzSquared);

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}

QColor AdaGrad::color()
{
    return Qt::cyan;
}


QString AdaGrad::name()
{
    return "AdaGrad";
}
