#include "vanillagradientdescent.h"

VanillaGradientDescent::VanillaGradientDescent()
    : GradientDescent()
{
}

float VanillaGradientDescent::updateRule(float xHat, float dfdx, float lr) {
    xHat -= lr * dfdx;
    return xHat;
}

vector<QVector3D> VanillaGradientDescent::run()
{
    int k = 1;
    qDebug() << "VanillaGradientDescent";
    qDebug() << "m_lr" << m_lr;
    qDebug() << "m_tol" << m_tol;
    qDebug() << "m_nIterMax" << m_nIterMax;

    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > m_tol) && (k <= (int)m_nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);

        m_xHat = updateRule(m_xHat, m_dfdx, m_lr);
        m_zHat = updateRule(m_zHat, m_dfdz, m_lr);

        m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

        k += 1;
    }
    return m_pointsTable;
}

QColor VanillaGradientDescent::color()
{
    return Qt::black;
}


QString VanillaGradientDescent::name()
{
    return "Vanilla Gradient Descent";
}

QList<QString> VanillaGradientDescent::hyperParameters() {
    return { "lr", "tol", "nIterMax" };
}
