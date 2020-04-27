#include "gradientdescentwithmomentum.h"

GradientDescentWithMomentum::GradientDescentWithMomentum()
    : GradientDescent()
{

}

vector<QVector3D> GradientDescentWithMomentum::run(float lr, float tol, int nIterMax)
{
    int k = 0;

    // Stanford University | Lecture 7
    // Build up “velocity” as a running mean of gradients
    // Rho gives “friction”; typically rho=0.9 or 0.99
    float rho = 0.9;
    float vx = 0;
    float vz = 0;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k < nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);

        vx = rho*vx + m_dfdx;
        m_xHat = m_xHat - lr * vx;

        vz = rho*vz + m_dfdz;
        m_zHat = m_zHat - lr * vz;

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}

QColor GradientDescentWithMomentum::color()
{
    return Qt::green;
}


QString GradientDescentWithMomentum::name()
{
    return "Gradient Descent With Momentum";
}

bool GradientDescentWithMomentum::curveIsDisplayed()
{
    return m_curveIsDisplayed;
}

void GradientDescentWithMomentum::setCurveIsDisplayed(bool curveIsDisplayed)
{
    m_curveIsDisplayed = curveIsDisplayed;
}
