#include "gradientdescent/gradientdescent.h"

int GradientDescent::static_id = 0;

GradientDescent::GradientDescent()
{
    setId(static_id);
    static_id++;

    m_statisticLabelToValue["Execution time (in seconds)"] = 0;
    m_statisticLabelToValue["Last cost value"] = 0;
    m_statisticLabelToValue["nIter"] = 0;
    m_statisticLabelToValue["nIterMax"] = 0;
}

void GradientDescent::initialize(SurfaceGraph *modifier, QVector3D initializationPoint)
{
    m_modifier = modifier;

    m_points.clear();

    m_xHat = initializationPoint.x(); // weight in Deep Learning
    m_zHat = initializationPoint.z(); // weight in Deep Learning
    m_cost = initializationPoint.y(); // <=> computeCostFunction(m_xHat, m_zHat);
    m_points.push_back(QVector3D(m_xHat, m_cost, m_zHat));

    // Initialize partial derivatives such as the norm of the gradient is greater
    // than the tolerance chosen in the while loop of the child class
    m_dfdx = 1; // partial derivative (gradient in Deep Learning)
    m_dfdz = 1; // partial derivative (gradient in Deep Learning)
}

float GradientDescent::evaluateF(float xHat, float zHat)
{
    return m_modifier->evaluateFunction("f", xHat, zHat);
}

float GradientDescent::evaluateDfdx(float xHat, float zHat)
{
    return m_modifier->evaluateFunction("dfdx", xHat, zHat);
}

float GradientDescent::evaluateDfdz(float xHat, float zHat)
{
    return m_modifier->evaluateFunction("dfdz", xHat, zHat);
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

void GradientDescent::setHyperParameterValue(QString hyperParameter, float value)
{
    m_hyperParameterToValue[hyperParameter] = value;
}

float GradientDescent::hyperParameterValue(QString hyperParameter)
{
    return m_hyperParameterToValue[hyperParameter];
}

float GradientDescent::hyperParameterDefaultValue(QString hyperParameter)
{
    return m_hyperParameterToDefaultValue[hyperParameter];
}

vector<QString> GradientDescent::hyperParameters()
{
    vector<QString> tab(m_hyperParameterToDefaultValue.size());

    int i = 0;
    tab[i] = "lr";
    i++;
    tab[i] = "tol";
    i++;
    tab[i] = "nIterMax";
    i++;
    // These previous lines are not nice but they allow the spinbox to appear in the
    // same order for each Gradient Descent Method

    map<QString, float>::iterator it;
    for (it = m_hyperParameterToDefaultValue.begin(); it != m_hyperParameterToDefaultValue.end(); ++it) {
        if (it->first != "lr" && it->first != "nIterMax" && it->first != "tol") {
            tab[i] = it->first;
            i++;
        }
    }

    return tab;
}

map<QString, float> GradientDescent::statisticLabelToValue()
{
    return m_statisticLabelToValue;
}

float GradientDescent::convertTime(high_resolution_clock::time_point start,
                                   high_resolution_clock::time_point end)
{
    // Calculating total time taken by the program.
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    return time_taken;
}
