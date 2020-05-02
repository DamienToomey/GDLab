#include "gradientdescent/nesterovmomentum.h"

NesterovMomentum::NesterovMomentum()
    : GradientDescent()
{
    m_hyperParameterToDefaultValue["lr"] = 1e-3;
    m_hyperParameterToDefaultValue["tol"] = 0.005;
    m_hyperParameterToDefaultValue["nIterMax"] = 10000;

    // Stanford University | Lecture 7
    // Build up “velocity” as a running mean of gradients
    // Rho gives “friction”; typically rho=0.9 or 0.99
    m_hyperParameterToDefaultValue["rho"] = 0.9;

    m_hyperParameterToValue = m_hyperParameterToDefaultValue;
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

    float lr = m_hyperParameterToValue["lr"];
    float tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];
    float rho = m_hyperParameterToValue["rho"];

    float vx = 0;
    float vz = 0;

    time_t start = time(0);
    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k <= nIterMax)) {
        m_cost = evaluateF(m_xHat, m_zHat);
        m_dfdx = evaluateDfdx(m_xHat, m_zHat);
        m_dfdz = evaluateDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, lr, rho, vx);
        m_zHat = updateRule(m_zHat, m_dfdz, lr, rho, vz);

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

QColor NesterovMomentum::color()
{
    return Qt::green;
}


QString NesterovMomentum::name()
{
    return "Nesterov Momentum";
}
