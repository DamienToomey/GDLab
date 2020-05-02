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

float Adam::updateRule(float xHat, float dfdx, float lr, float beta1, float beta2,
                       float& firstMoment, float& secondMoment, int k)
{
    firstMoment = beta1 * firstMoment + (1 - beta1) * dfdx;
    secondMoment = beta2 * secondMoment + (1 - beta2) * dfdx * dfdx;
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

    time_t start = time(0);
    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > tol) && (k <= nIterMax)) {
        m_cost = evaluateF(m_xHat, m_zHat);
        m_dfdx = evaluateDfdx(m_xHat, m_zHat);
        m_dfdz = evaluateDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, lr, beta1, beta2,
                            firstMomentX, secondMomentX, k);
        m_zHat = updateRule(m_zHat, m_dfdz, lr, beta1, beta2,
                            firstMomentZ, secondMomentZ, k);

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

QColor Adam::color()
{
    return Qt::magenta;
}


QString Adam::name()
{
    return "Adam";
}
