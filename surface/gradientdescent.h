#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include "mainwindow.h"
#include <QtDataVisualization/QCustom3DItem>

#include<cmath>

class GradientDescent
{
public:
    GradientDescent(MainWindow *mainWindow);
    virtual float run() = 0; // pure virtual method
    float computeCostFunction(float xHat, float zHat);
    float computeDfdx(float xHat, float zHat);
    float computeDfdz(float xHat, float zHat);
    void plotPoint(int k);

protected:
    SurfaceGraph *m_modifier;
    float m_xHat;
    float m_zHat;
    float m_dfdx;
    float m_dfdz;
    int m_nIterMax;
    float m_lr;
    float m_tol;
    float m_cost;
    QJSValue m_costFunctionEngine;
    QJSValue m_dfdxEngine;
    QJSValue m_dfdzEngine;
    MainWindow *m_mainWindow;
    Q3DSurface *m_graph;
    QJSEngine m_engine;
};

#endif // GRADIENTDESCENT_H
