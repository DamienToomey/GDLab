#include "gradientdescent/gradientdescent.h"

int GradientDescent::static_id = 0;

GradientDescent::GradientDescent()
{
    setId(static_id);
    static_id++;

    setStatistics(0, 0, 0, 0, 0, 0, 0);
}

void GradientDescent::initialize(SurfaceGraph *modifier, QVector3D initializationPoint)
{
    m_modifier = modifier;

    m_points.clear();

    m_xHat = initializationPoint.x(); // weight in Deep Learning
    m_cost = initializationPoint.y(); // <=> computeCostFunction(m_xHat, m_zHat);
    m_zHat = initializationPoint.z(); // weight in Deep Learning
    m_points.push_back(QVector3D(m_xHat, m_cost, m_zHat));

    // Initialize partial derivatives such as the norm of the gradient is greater
    // than the tolerance chosen in the while loop of the child class
    m_dx = 1; // partial derivative (gradient in Deep Learning)
    m_dz = 1; // partial derivative (gradient in Deep Learning)
}

double GradientDescent::evaluateF(double xHat, double zHat)
{
    return m_modifier->evaluateFunction("f", xHat, zHat);
}

double GradientDescent::evaluateDx(double xHat, double zHat)
{
    return m_modifier->evaluateFunction("dx", xHat, zHat);
}

double GradientDescent::evaluateDz(double xHat, double zHat)
{
    return m_modifier->evaluateFunction("dz", xHat, zHat);
}

double GradientDescent::evaluateDxdx(double xHat, double zHat)
{
    return m_modifier->evaluateFunction("dxdx", xHat, zHat);
}

double GradientDescent::evaluateDxdz(double xHat, double zHat)
{
    return m_modifier->evaluateFunction("dxdz", xHat, zHat);
}

double GradientDescent::evaluateDzdx(double xHat, double zHat)
{
    return m_modifier->evaluateFunction("dzdx", xHat, zHat);
}

double GradientDescent::evaluateDzdz(double xHat, double zHat)
{
    return m_modifier->evaluateFunction("dzdz", xHat, zHat);
}


vector<QVector3D> GradientDescent::points()
{
    return m_points;
}

void GradientDescent::setId(int id)
{
    m_id = id;
}

int GradientDescent::id()
{
    return m_id;
}

GradientDescent::~GradientDescent()
{
    delete m_modifier;
}

void GradientDescent::setHyperParameterValue(QString hyperParameter, double value)
{
    m_hyperParameterToValue[hyperParameter] = value;
}

double GradientDescent::hyperParameterValue(QString hyperParameter)
{
    return m_hyperParameterToValue[hyperParameter];
}

double GradientDescent::hyperParameterDefaultValue(QString hyperParameter)
{
    return m_hyperParameterToDefaultValue[hyperParameter];
}

vector<QString> GradientDescent::hyperParameters()
{
    vector<QString> tab(m_hyperParameterToDefaultValue.size());

    int i = 0;
    if (m_hyperParameterToDefaultValue.find("lr") != m_hyperParameterToDefaultValue.end()) {
        tab[i] = "lr";
        i++;
    }

    if (m_hyperParameterToDefaultValue.find("tol") != m_hyperParameterToDefaultValue.end()) {
        tab[i] = "tol";
        i++;
    }

    if (m_hyperParameterToDefaultValue.find("nIterMax") != m_hyperParameterToDefaultValue.end()) {
        tab[i] = "nIterMax";
        i++;
    }

    // These previous lines are not nice but they allow the spinbox to appear in the
    // same order for each Gradient Descent Method

    map<QString, double>::iterator it;
    for (it = m_hyperParameterToDefaultValue.begin(); it != m_hyperParameterToDefaultValue.end(); ++it) {
        if (it->first != "lr" && it->first != "nIterMax" && it->first != "tol") {
            tab[i] = it->first;
            i++;
        }
    }

    return tab;
}

map<QString, double> GradientDescent::statisticLabelToValue()
{
    return m_statisticLabelToValue;
}

double GradientDescent::convertTime(high_resolution_clock::time_point start,
                                   high_resolution_clock::time_point end)
{
    // Calculating total time taken by the program.
    double time_taken = chrono::duration_cast<nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    return time_taken;
}

bool GradientDescent::hessianIsNecessary() {
    return false;
}

void GradientDescent::setStatistics(double executionTime, double xHat, double lastCostValue,
                                    double zHat, int nIter, int nIterMax, bool prematureStop) {
    m_statisticLabelToValue["Execution time (in seconds)"] = executionTime;
    m_statisticLabelToValue["xHat"] = xHat;
    m_statisticLabelToValue["Last cost value"] = lastCostValue;
    m_statisticLabelToValue["zHat"] = zHat;
    m_statisticLabelToValue["nIter"] = nIter;
    m_statisticLabelToValue["nIterMax"] = nIterMax;
    m_statisticLabelToValue["Premature stop (inf, -inf or nan)"] = prematureStop;
}
