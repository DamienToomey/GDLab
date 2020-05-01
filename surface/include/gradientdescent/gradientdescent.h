#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include "include/surfacegraph.h"

#include<cmath>
#include <vector>

class GradientDescent
{
public:
    GradientDescent();
    virtual ~GradientDescent();
    virtual vector<QVector3D> run() = 0; // pure virtual method
    virtual QList<QString> hyperParameters() = 0;
    virtual QColor color() = 0;
    virtual QString name() = 0;
    void initialize(SurfaceGraph *modifier, QVector3D selectedPoint);
    float computeCostFunction(float xHat, float zHat);
    float computeDfdx(float xHat, float zHat);
    float computeDfdz(float xHat, float zHat);
    int id();    
    vector<QVector3D> points();
    typedef void (GradientDescent::*setterFunction)(float x);
    map<QString, setterFunction> hyperParameterToSetter();

protected:
    float m_xHat;
    float m_zHat;
    float m_dfdx;
    float m_dfdz;
    float m_cost;
    float m_lr;
    float m_tol;
    float m_nIterMax;
    float m_decayRate;
    float m_beta1;
    float m_beta2;
    float m_rho;
    int m_id;
    vector<QVector3D> m_points;

    virtual float lr();
    virtual void setLr(float lr);
    virtual float tol();
    virtual void setTol(float tol);
    virtual float nIterMax();
    virtual void setNIterMax(float nIterMax);
    virtual float beta1();
    virtual void setBeta1(float beta1);
    virtual float beta2();
    virtual void setBeta2(float beta2);
    virtual float decayRate();
    virtual void setDecayRate(float decayRate);
    virtual float rho();
    virtual void setRho(float rho);

private:
    static int static_id;
    QJSValue m_costFunctionEngine;
    QJSValue m_dfdxEngine;
    QJSValue m_dfdzEngine;
    QJSEngine m_engine;
    SurfaceGraph *m_modifier;
    map<QString, setterFunction> m_hyperParameterToSetter;
    void setId(int id);
};

#endif // GRADIENTDESCENT_H
