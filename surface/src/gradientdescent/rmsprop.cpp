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

float RMSProp::updateRule(float xHat, float dx, float lr, float decayRate, float& dxSquared) {
    dxSquared = decayRate * dxSquared + (1 - decayRate) * dx * dx;
    xHat -= lr * dx / (sqrt(dxSquared) + 1e-7);
    return xHat;
}

vector<QVector3D> RMSProp::run()
{
    int k = 0;

    float lr = m_hyperParameterToValue["lr"];
    float tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];
    float decayRate = m_hyperParameterToValue["decayRate"];

    float dxSquared = 0;
    float dzSquared = 0;

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

        if (isnan(m_cost) or isinf(m_cost)) {
            prematureStop = true;
            break;
        }
        else {
            m_points.push_back(QVector3D(m_xHat, m_cost, m_zHat));
        }
    }

    high_resolution_clock::time_point end = high_resolution_clock::now();
    float time_taken = convertTime(start, end);

    m_statisticLabelToValue["Execution time (in seconds)"] = time_taken;
    m_statisticLabelToValue["xHat"] = m_points.back().x();
    m_statisticLabelToValue["Last cost value"] = m_points.back().y();
    m_statisticLabelToValue["zHat"] = m_points.back().z();
    m_statisticLabelToValue["nIter"] = k - 1;
    m_statisticLabelToValue["nIterMax"] = nIterMax;
    m_statisticLabelToValue["Premature stop (inf, -inf or nan)"] = prematureStop;

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
