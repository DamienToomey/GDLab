#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include "surfacegraph.h"
#include <QtDataVisualization/QCustom3DItem>

#include<cmath>
#include <vector>

class GradientDescent
{
public:
    GradientDescent(SurfaceGraph *modifier, QVector3D selectedPoint);
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax) = 0; // pure virtual method
    float computeCostFunction(float xHat, float zHat);
    float computeDfdx(float xHat, float zHat);
    float computeDfdz(float xHat, float zHat);
    vector<QVector3D> pointsTable();

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
};

#endif // GRADIENTDESCENT_H
