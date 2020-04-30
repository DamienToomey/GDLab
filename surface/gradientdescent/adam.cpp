#include "adam.h"

Adam::Adam()
    : GradientDescent()
{
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

    // Stanford University | Lecture 7
    // "Adam with beta1 = 0.9, beta2 = 0.999, and learning_rate = 1e-3 or 5e-4
    // is a great starting point for many models!"
//    float beta1 = 0.9;
//    float beta2 = 0.999;

    float firstMomentX = 0;
    float secondMomentX = 0;
    float firstMomentZ = 0;
    float secondMomentZ = 0;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > m_tol) && (k <= (int)m_nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, m_lr, m_beta1, m_beta2,
                            firstMomentX, secondMomentX, k);
        m_zHat = updateRule(m_zHat, m_dfdz, m_lr, m_beta1, m_beta2,
                            firstMomentZ, secondMomentZ, k);

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}

QColor Adam::color()
{
    return Qt::magenta;
}


QString Adam::name()
{
    return "Adam";
}

QList<QString> Adam::hyperParameters() {
    return { "lr", "tol", "beta1", "beta2", "nIterMax" };
}
