#include "gradientdescent/gradientdescentwithmomentum.h"

GradientDescentWithMomentum::GradientDescentWithMomentum()
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

double GradientDescentWithMomentum::updateRule(double xHat, double dx, double lr, double rho, double& vx) {
    vx = rho * vx + dx;
    xHat -= lr * vx;
    return xHat;
}

vector<QVector3D> GradientDescentWithMomentum::run()
{
    int k = 1;

    double lr = m_hyperParameterToValue["lr"];
    double tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];
    double rho = m_hyperParameterToValue["rho"];

    double vx = 0;
    double vz = 0;

    bool prematureStop = false;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    // unsync the I/O of C and C++.
    ios_base::sync_with_stdio(false);

    while ((sqrt(pow(m_dx, 2) + pow(m_dz, 2)) > tol) && (k <= nIterMax)) {        
        m_dx = evaluateDx(m_xHat, m_zHat);
        m_dz = evaluateDz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dx, lr, rho, vx);
        m_zHat = updateRule(m_zHat, m_dz, lr, rho, vz);

        m_cost = evaluateF(m_xHat, m_zHat);

        k += 1;

        if (isnan((float)m_cost) or isinf((float)m_cost)) {
            // QVector3D takes float as input so if double value
            // is too large it will be converted to nan, inf or -inf
            prematureStop = true;
            break;
        }
        else {
            m_points.push_back(QVector3D(m_xHat, m_cost, m_zHat));
        }
    }

    high_resolution_clock::time_point end = high_resolution_clock::now();
    double time_taken = convertTime(start, end);

    setStatistics(time_taken, m_points.back().x(), m_points.back().y(),
                  m_points.back().z(), k - 1, nIterMax, prematureStop);

    return m_points;
}

QColor GradientDescentWithMomentum::color()
{
    return Qt::blue;
}


QString GradientDescentWithMomentum::name()
{
    return "GD With Momentum";
}

bool GradientDescentWithMomentum::hessianIsNecessary() {
    return false;
}
