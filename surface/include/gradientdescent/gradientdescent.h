#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include "surfacegraph.h"

#include <chrono>
using namespace chrono;

#include <cmath>
#include <vector>

class GradientDescent
{
public:
    GradientDescent();
    virtual ~GradientDescent();
    virtual vector<QVector3D> run() = 0;
    virtual QColor color() = 0;
    virtual QString name() = 0;
    virtual bool hessianIsNecessary();
    void setHyperParameterValue(QString hyperParameter, float value);
    float hyperParameterValue(QString hyperParameter);
    void initialize(SurfaceGraph *modifier, QVector3D selectedPoint);
    float evaluateF(float xHat, float zHat);
    float evaluateDx(float xHat, float zHat);
    float evaluateDz(float xHat, float zHat);
    int id();    
    vector<QVector3D> points();
    float hyperParameterDefaultValue(QString hyperParameter);
    vector<QString> hyperParameters();
    map<QString, float> statisticLabelToValue();
    float evaluateDxdx(float xHat, float zHat);
    float evaluateDxdz(float xHat, float zHat);
    float evaluateDzdx(float xHat, float zHat);
    float evaluateDzdz(float xHat, float zHat);

protected:
    float m_xHat;
    float m_zHat;
    float m_dx;
    float m_dz;
    float m_dxdx;
    float m_dxdz;
    float m_dzdx;
    float m_dzdz;
    float m_cost;
    int m_id;
    vector<QVector3D> m_points;
    map<QString, float> m_hyperParameterToValue;
    map<QString, float> m_hyperParameterToDefaultValue;
    map<QString, float> m_statisticLabelToValue;
    float convertTime(high_resolution_clock::time_point start, high_resolution_clock::time_point end);

private:
    static int static_id;
    SurfaceGraph *m_modifier;
    void setId(int id);
};

#endif // GRADIENTDESCENT_H
