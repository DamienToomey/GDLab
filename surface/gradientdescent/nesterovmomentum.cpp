#include "nesterovmomentum.h"

NesterovMomentum::NesterovMomentum(SurfaceGraph *modifier, QVector3D selectedPoint)
    : GradientDescent(modifier, selectedPoint)
{

}

vector<QVector3D> NesterovMomentum::run(float lr, float tol, int nIterMax)
{
    int k = 0;

    // Stanford University | Lecture 7
    // Build up “velocity” as a running mean of gradients
    // Rho gives “friction”; typically rho=0.9 or 0.99
    float rho = 0.9;
    float vx = 0;
    float vz = 0;
    float old_vx;
    float old_vz;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k < nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);

        old_vx = vx;
        vx = rho * vx - lr * m_dfdx;
        m_xHat = m_xHat - rho * old_vx + (1 + rho) * vx;

        old_vz = vz;
        vz = rho * vz - lr * m_dfdz;
        m_zHat = m_zHat - rho * old_vz + (1 + rho) * vz;

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}
