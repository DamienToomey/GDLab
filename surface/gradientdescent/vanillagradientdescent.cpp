#include "vanillagradientdescent.h"

VanillaGradientDescent::VanillaGradientDescent(SurfaceGraph *modifier, QVector3D selectedPoint)
    : GradientDescent(modifier, selectedPoint)
{
}

vector<QVector3D> VanillaGradientDescent::run(float lr, float tol, int nIterMax)
{
    int k = 0;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k < nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);
        m_xHat = m_xHat - lr * m_dfdx;
        m_zHat = m_zHat - lr * m_dfdz;

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}


