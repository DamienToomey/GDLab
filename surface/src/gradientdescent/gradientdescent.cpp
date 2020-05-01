#include "gradientdescent/gradientdescent.h"

int GradientDescent::static_id = 0;

GradientDescent::GradientDescent()
{
    setId(static_id);
    static_id++;

    m_hyperParameterToSetter["lr"] = &GradientDescent::setLr;
    m_hyperParameterToSetter["tol"] = &GradientDescent::setTol;
    m_hyperParameterToSetter["nIterMax"] = &GradientDescent::setNIterMax;
    m_hyperParameterToSetter["beta1"] = &GradientDescent::setBeta1;
    m_hyperParameterToSetter["beta2"] = &GradientDescent::setBeta2;
    m_hyperParameterToSetter["decayRate"] = &GradientDescent::setDecayRate;
    m_hyperParameterToSetter["rho"] = &GradientDescent::setRho;
}

void GradientDescent::initialize(SurfaceGraph *modifier, QVector3D initializationPoint)
{
    m_modifier = modifier;
    m_costFunctionEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->costFunction()));
    m_dfdxEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->dfdx()));
    m_dfdzEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->dfdz()));

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

float GradientDescent::computeCostFunction(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_costFunctionEngine.call(args).toNumber();
}

float GradientDescent::computeDfdx(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_dfdxEngine.call(args).toNumber();
}

float GradientDescent::computeDfdz(float xHat, float zHat)
{
    QJSValueList args;
    args << xHat << zHat;
    return m_dfdzEngine.call(args).toNumber();
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

float GradientDescent::lr() {
    return m_lr;
}

void GradientDescent::setLr(float lr) {
    m_lr = lr;
}

float GradientDescent::tol() {
    return m_tol;
}

void GradientDescent::setTol(float tol) {
    m_tol = tol;
}

float GradientDescent::nIterMax() {
    return m_nIterMax;
}

void GradientDescent::setNIterMax(float nIterMax) {
    m_nIterMax = nIterMax;
}

float GradientDescent::beta1() {
    return m_beta1;
}

void GradientDescent::setBeta1(float beta1) {
    m_beta1 = beta1;
}

float GradientDescent::beta2() {
    return m_beta2;
}

void GradientDescent::setBeta2(float beta2) {
    m_beta2 = beta2;
}

float GradientDescent::decayRate() {
    return m_decayRate;
}

void GradientDescent::setDecayRate(float decayRate) {
    m_decayRate = decayRate;
}

float GradientDescent::rho() {
    return m_rho;
}

void GradientDescent::setRho(float rho) {
    m_rho = rho;
}

map<QString, GradientDescent::setterFunction> GradientDescent::hyperParameterToSetter()
{
    return m_hyperParameterToSetter;
}

GradientDescent::~GradientDescent()
{
    delete m_modifier;
}
