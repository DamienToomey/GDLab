#include "vanillagradientdescent.h"

VanillaGradientDescent::VanillaGradientDescent(MainWindow *mainWindow)
    : GradientDescent(mainWindow)
{
}

float VanillaGradientDescent::run()
{
    int k = 0;
    plotPoint(k);
    while ((sqrt(pow(m_dfdx, 2) + pow(m_dfdz, 2)) > m_tol) && (k < m_nIterMax)) {
        m_cost = computeCostFunction(m_xHat, m_zHat);
        m_dfdx = computeDfdx(m_xHat, m_zHat);
        m_dfdz = computeDfdz(m_xHat, m_zHat);
        m_xHat = m_xHat-m_lr*m_dfdx;
        m_zHat = m_zHat-m_lr*m_dfdz;

        plotPoint(k);

        k = k+1;
    }
    plotPoint(k); // WARNING : could plot same point twice
    return k;
}


