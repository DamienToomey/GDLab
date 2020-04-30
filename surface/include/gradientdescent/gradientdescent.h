#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include "include/surfacegraph.h"
#include <QtDataVisualization/QCustom3DItem>

#include<cmath>
#include <vector>

class GradientDescent
{
public:
    GradientDescent();
    virtual vector<QVector3D> run() = 0; // pure virtual method
    float computeCostFunction(float xHat, float zHat);
    float computeDfdx(float xHat, float zHat);
    float computeDfdz(float xHat, float zHat);
    vector<QVector3D> pointsTable();
    virtual QColor color() = 0;
    virtual QString name() = 0;
    void initialize(SurfaceGraph *modifier, QVector3D selectedPoint);
    QQuaternion rotation();
    void setRotation(int curveId);
    virtual QList<QString> hyperParameters() = 0;
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
    typedef void (GradientDescent::*setterFunction)(float x);
    typedef float (GradientDescent::*getterFunction)();
    map<QString, setterFunction> hyperParameterToSetter();
    map<QString, getterFunction> hyperParameterToGetter();

protected:
    SurfaceGraph *m_modifier;
    float m_xHat;
    float m_zHat;
    float m_dfdx;
    float m_dfdz;
    float m_cost;
    QJSValue m_costFunctionEngine;
    QJSValue m_dfdxEngine;
    QJSValue m_dfdzEngine;
    QJSEngine m_engine;
    vector<QVector3D> m_pointsTable;
    static int m_curveId;
    QQuaternion m_rotation;
    float m_lr;
    float m_tol;
    float m_nIterMax;
    float m_decayRate;
    float m_beta1;
    float m_beta2;
    float m_rho;
    map<QString, setterFunction> m_hyperParameterToSetter;
    map<QString, getterFunction> m_hyperParameterToGetter;
};

#endif // GRADIENTDESCENT_H
