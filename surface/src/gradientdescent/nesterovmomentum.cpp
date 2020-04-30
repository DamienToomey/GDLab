#include "gradientdescent/nesterovmomentum.h"

NesterovMomentum::NesterovMomentum()
    : GradientDescent()
{
}

float NesterovMomentum::updateRule(float xHat, float dfdx, float lr, float rho, float& vx) {
    float old_vx = vx;
    vx = rho * vx - lr * dfdx;
    xHat += -rho * old_vx + (1 + rho) * vx;
    return xHat;
}

vector<QVector3D> NesterovMomentum::run()
{
    int k = 1;

    // Stanford University | Lecture 7
    // Build up “velocity” as a running mean of gradients
    // Rho gives “friction”; typically rho=0.9 or 0.99
//    float rho = 0.9;
    float vx = 0;
    float vz = 0;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > m_tol) && (k <= (int)m_nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, m_lr, m_rho, vx);
        m_zHat = updateRule(m_zHat, m_dfdz, m_lr, m_rho, vz);

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}

QColor NesterovMomentum::color()
{
    return Qt::green;
}


QString NesterovMomentum::name()
{
    return "Nesterov Momentum";
}

QList<QString> NesterovMomentum::hyperParameters() {
    return { "lr", "tol", "rho", "nIterMax" };
}
