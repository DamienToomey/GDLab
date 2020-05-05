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

double Adam::updateRule(double xHat, double dx, double lr, double beta1, double beta2,
                        double& firstMoment, double& secondMoment, int k)
{
    firstMoment = beta1 * firstMoment + (1 - beta1) * dx;
    secondMoment = beta2 * secondMoment + (1 - beta2) * dx * dx;
    double firstUnbias = firstMoment / (1 - pow(beta1, k));
    double secondUnbias = secondMoment / (1 - pow(beta2, k));
    xHat -= lr * firstUnbias / (sqrt(secondUnbias) + 1e-7);
    return xHat;
}

vector<QVector3D> Adam::run()
{
    int k = 1;

    double lr = m_hyperParameterToValue["lr"];
    double tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];
    double beta1 = m_hyperParameterToValue["beta1"];
    double beta2 = m_hyperParameterToValue["beta2"];

    double firstMomentX = 0;
    double secondMomentX = 0;
    double firstMomentZ = 0;
    double secondMomentZ = 0;

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

        if (isnan((float)m_cost) or isinf((float)m_cost)) {
            // QVector3D takes float as input so if double value
            // is too large it will converted to nan, inf or -inf
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

QColor Adam::color()
{
    return Qt::magenta;
}


QString Adam::name()
{
    return "Adam";
}

bool Adam::hessianIsNecessary() {
    return false;
}
