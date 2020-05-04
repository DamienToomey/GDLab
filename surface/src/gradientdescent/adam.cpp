#include "gradientdescent/adam.h"

Adam::Adam()
    : GradientDescent()
{
    m_hyperParameterToDefaultValue["lr"] = 1e-3;
    m_hyperParameterToDefaultValue["tol"] = 0.005;
    m_hyperParameterToDefaultValue["nIterMax"] = 10000;

    // Stanford University | Lecture 7
    // "Adam with beta1 = 0.9, beta2 = 0.999, and learning_rate = 1e-3 or 5e-4
    // is a great starting point for many models!"
    m_hyperParameterToDefaultValue["beta1"] = 0.9;
    m_hyperParameterToDefaultValue["beta2"] = 0.999;

    m_hyperParameterToValue = m_hyperParameterToDefaultValue;
}

float Adam::updateRule(float xHat, float dx, float lr, float beta1, float beta2,
                       float& firstMoment, float& secondMoment, int k)
{
    firstMoment = beta1 * firstMoment + (1 - beta1) * dx;
    secondMoment = beta2 * secondMoment + (1 - beta2) * dx * dx;
    float firstUnbias = firstMoment / (1 - pow(beta1, k));
    float secondUnbias = secondMoment / (1 - pow(beta2, k));
    xHat -= lr * firstUnbias / (sqrt(secondUnbias) + 1e-7);
    return xHat;
}

vector<QVector3D> Adam::run()
{
    int k = 1;

    float lr = m_hyperParameterToValue["lr"];
    float tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];
    float beta1 = m_hyperParameterToValue["beta1"];
    float beta2 = m_hyperParameterToValue["beta2"];

    float firstMomentX = 0;
    float secondMomentX = 0;
    float firstMomentZ = 0;
    float secondMomentZ = 0;

    bool prematureStop = false;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    // unsync the I/O of C and C++.
    ios_base::sync_with_stdio(false);

    while ((sqrt(pow(m_dx, 2) + pow(m_dz, 2)) > tol) && (k <= nIterMax)) {
        m_dx = evaluateDx(m_xHat, m_zHat);
        m_dz = evaluateDz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dx, lr, beta1, beta2,
                            firstMomentX, secondMomentX, k);
        m_zHat = updateRule(m_zHat, m_dz, lr, beta1, beta2,
                            firstMomentZ, secondMomentZ, k);

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

QColor Adam::color()
{
    return Qt::magenta;
}


QString Adam::name()
{
    return "Adam";
}
