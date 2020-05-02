#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include "surfacegraph.h"

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
    void setHyperParameterValue(QString hyperParameter, float value);
    float hyperParameterValue(QString hyperParameter);
    void initialize(SurfaceGraph *modifier, QVector3D selectedPoint);
    float evaluateF(float xHat, float zHat);
    float evaluateDfdx(float xHat, float zHat);
    float evaluateDfdz(float xHat, float zHat);
    int id();    
    vector<QVector3D> points();
    float hyperParameterDefaultValue(QString hyperParameter);
    vector<QString> hyperParameters();
    map<QString, float> statisticLabelToValue();

protected:
    float m_xHat;
    float m_zHat;
    float m_dfdx;
    float m_dfdz;
    float m_cost;
    int m_id;
    vector<QVector3D> m_points;
    map<QString, float> m_hyperParameterToValue;
    map<QString, float> m_hyperParameterToDefaultValue;
    map<QString, float> m_statisticLabelToValue;

private:
    static int static_id;
    SurfaceGraph *m_modifier;
    void setId(int id);
};

#endif // GRADIENTDESCENT_H
