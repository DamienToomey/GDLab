#include "gradientdescentwithmomentum.h"

GradientDescentWithMomentum::GradientDescentWithMomentum()
    : GradientDescent()
{
}

float GradientDescentWithMomentum::updateRule(float xHat, float dfdx, float lr, float rho, float& vx) {
    vx = rho * vx + dfdx;
    xHat -= lr * vx;
    return xHat;
}

vector<QVector3D> GradientDescentWithMomentum::run(float lr, float tol, int nIterMax)
{
    int k = 1;

    // Stanford University | Lecture 7
    // Build up “velocity” as a running mean of gradients
    // Rho gives “friction”; typically rho=0.9 or 0.99
    float rho = 0.9;
    float vx = 0;
    float vz = 0;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k <= nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, lr, rho, vx);
        m_zHat = updateRule(m_zHat, m_dfdz, lr, rho, vz);

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}

QColor GradientDescentWithMomentum::color()
{
    return Qt::blue;
}


QString GradientDescentWithMomentum::name()
{
    return "Gradient Descent With Momentum";
}
