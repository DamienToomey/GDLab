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
    virtual bool hessianIsNecessary() = 0;
    void setStatistics(double executionTime, double xHat, double lastCostValue,
                       double zHat, int nIter, int nIterMax, bool prematureStop);
    void setHyperParameterValue(QString hyperParameter, double value);
    double hyperParameterValue(QString hyperParameter);
    void initialize(SurfaceGraph *modifier, QVector3D selectedPoint);
    double evaluateF(double xHat, double zHat);
    double evaluateDx(double xHat, double zHat);
    double evaluateDz(double xHat, double zHat);
    int id();    
    vector<QVector3D> points();
    double hyperParameterDefaultValue(QString hyperParameter);
    vector<QString> hyperParameters();
    map<QString, double> statisticLabelToValue();
    double evaluateDxdx(double xHat, double zHat);
    double evaluateDxdz(double xHat, double zHat);
    double evaluateDzdx(double xHat, double zHat);
    double evaluateDzdz(double xHat, double zHat);

protected:
    double m_xHat;
    double m_zHat;
    double m_dx;
    double m_dz;
    double m_dxdx;
    double m_dxdz;
    double m_dzdx;
    double m_dzdz;
    double m_cost;
    int m_id;
    vector<QVector3D> m_points;
    map<QString, double> m_hyperParameterToValue;
    map<QString, double> m_hyperParameterToDefaultValue;
    map<QString, double> m_statisticLabelToValue;
    double convertTime(high_resolution_clock::time_point start, high_resolution_clock::time_point end);

private:
    static int static_id;
    SurfaceGraph *m_modifier;
    void setId(int id);
};

#endif // GRADIENTDESCENT_H
