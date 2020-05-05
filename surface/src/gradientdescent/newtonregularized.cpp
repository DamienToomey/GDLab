#include "gradientdescent/newtonregularized.h"

NewtonRegularized::NewtonRegularized()
    : GradientDescent()
{
    m_hyperParameterToDefaultValue["tol"] = 0.005;
    m_hyperParameterToDefaultValue["nIterMax"] = 10000;

    m_hyperParameterToValue = m_hyperParameterToDefaultValue;
}

Vector2d NewtonRegularized::updateRule(Vector2d wHat, Vector2d g, Matrix2d H, Matrix2d I, double lamb)
{
    Matrix2d temp = H + lamb * I;
    Vector2d direction = temp.colPivHouseholderQr().solve(g); // <=> temp.inverse() * g
    // See comparative table here : https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
    // for solving techniques
    wHat -= direction;
    return wHat;
}

vector<QVector3D> NewtonRegularized::run()
{
    int k = 1;

    double tol = m_hyperParameterToValue["tol"];
    int nIterMax  = (int)m_hyperParameterToValue["nIterMax"];

    bool prematureStop = false;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    // unsync the I/O of C and C++.
    ios_base::sync_with_stdio(false);

    double lamb = sqrt(numeric_limits<double>::epsilon());

    Matrix2d I = MatrixXd::Identity(2, 2);
    Vector2d g;
    Matrix2d H;
    Vector2d wHat;

    while ((sqrt(pow(m_dx, 2) + pow(m_dz, 2)) > tol) && (k <= nIterMax)) {
        m_dx = evaluateDx(m_xHat, m_zHat);
        m_dz = evaluateDz(m_xHat, m_zHat);
        m_dxdx = evaluateDxdx(m_xHat, m_zHat);
        m_dxdz = evaluateDxdz(m_xHat, m_zHat);
        m_dzdx = evaluateDzdx(m_xHat, m_zHat);
        m_dzdz = evaluateDzdz(m_xHat, m_zHat);
        wHat << m_xHat, m_zHat;

        g << m_dx, m_dz;

        H << m_dxdx, m_dxdz,
             m_dzdx, m_dzdz;

        wHat = updateRule(wHat, g, H, I, lamb);

        m_xHat = wHat(0);
        m_zHat = wHat(1);

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

QColor NewtonRegularized::color()
{
    return QColor(255, 165, 0); // orange
}


QString NewtonRegularized::name()
{
    return "Newton Regularized";
}

bool NewtonRegularized::hessianIsNecessary() {
    return true;
}
