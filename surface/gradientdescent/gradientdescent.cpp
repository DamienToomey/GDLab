#include "gradientdescent.h"

int GradientDescent::m_curveId = 0;

GradientDescent::GradientDescent()
{
    setRotation(m_curveId);
    GradientDescent::m_curveId++;

    m_hyperParameterToSetter["lr"] = &GradientDescent::setLr;
    m_hyperParameterToSetter["tol"] = &GradientDescent::setTol;
    m_hyperParameterToSetter["nIterMax"] = &GradientDescent::setNIterMax;
    m_hyperParameterToSetter["beta1"] = &GradientDescent::setBeta1;
    m_hyperParameterToSetter["beta2"] = &GradientDescent::setBeta2;
    m_hyperParameterToSetter["decayRate"] = &GradientDescent::setDecayRate;
    m_hyperParameterToSetter["rho"] = &GradientDescent::setRho;

    m_hyperParameterToGetter["lr"] = &GradientDescent::lr;
    m_hyperParameterToGetter["tol"] = &GradientDescent::tol;
    m_hyperParameterToGetter["nIterMax"] = &GradientDescent::nIterMax;
    m_hyperParameterToGetter["beta1"] = &GradientDescent::beta1;
    m_hyperParameterToGetter["beta2"] = &GradientDescent::beta2;
    m_hyperParameterToGetter["decayRate"] = &GradientDescent::decayRate;
    m_hyperParameterToGetter["rho"] = &GradientDescent::rho;
}

void GradientDescent::initialize(SurfaceGraph *modifier, QVector3D initializationPoint)
{
    m_modifier = modifier;
    m_costFunctionEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->costFunction()));
    m_dfdxEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->dfdx()));
    m_dfdzEngine = m_engine.evaluate(QString("(function(x, z) { return %1 ; })").arg(m_modifier->dfdz()));

    m_pointsTable.clear();

    m_xHat = initializationPoint.x(); // weight in Deep Learning
    m_zHat = initializationPoint.z(); // weight in Deep Learning
    m_cost = initializationPoint.y(); // <=> computeCostFunction(m_xHat, m_zHat);
    m_pointsTable.push_back(QVector3D(m_xHat, m_cost, m_zHat));

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

vector<QVector3D> GradientDescent::pointsTable()
{
    return m_pointsTable;
}

void GradientDescent::setRotation(int curveId)
{
    m_rotation = QQuaternion::fromAxisAndAngle(curveId, curveId, curveId, curveId);
}

QQuaternion GradientDescent::rotation()
{
    return m_rotation;
}

bool GradientDescent::curveIsDisplayed()
{
    return m_curveIsDisplayed;
}

void GradientDescent::setCurveIsDisplayed(bool curveIsDisplayed)
{
    m_curveIsDisplayed = curveIsDisplayed;
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

map<QString, GradientDescent::getterFunction> GradientDescent::hyperParameterToGetter()
{
    return m_hyperParameterToGetter;
}
