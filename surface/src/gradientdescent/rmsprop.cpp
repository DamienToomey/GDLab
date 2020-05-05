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

double RMSProp::updateRule(double xHat, double dx, double lr, double decayRate, double& dxSquared) {
    dxSquared = decayRate * dxSquared + (1 - decayRate) * dx * dx;
    xHat -= lr * dx / (sqrt(dxSquared) + 1e-7);
    return xHat;
}

vector<QVector3D> RMSProp::run()
{
    int k = 0;

    double lr = m_hyperParameterToValue["lr"];
    double tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];
    double decayRate = m_hyperParameterToValue["decayRate"];

    double dxSquared = 0;
    double dzSquared = 0;

    bool prematureStop = false;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    // unsync the I/O of C and C++.
    ios_base::sync_with_stdio(false);

    while ((sqrt(pow(m_dx, 2) + pow(m_dz, 2)) > tol) && (k <= nIterMax)) {        
        m_dx = evaluateDx(m_xHat, m_zHat);
        m_dz = evaluateDz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dx, lr, decayRate, dxSquared);
        m_zHat = updateRule(m_zHat, m_dz, lr, decayRate, dzSquared);

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

QColor RMSProp::color()
{
    return Qt::red;
}


QString RMSProp::name()
{
    return "RMSProp";
}
