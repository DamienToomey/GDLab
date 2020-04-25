#ifndef VANILLAGRADIENTDESCENT_H
#define VANILLAGRADIENTDESCENT_H

#include "surfacegraph.h"
#include "mainwindow.h"
#include <QtDataVisualization/QCustom3DItem>

#include<cmath>

class VanillaGradientDescent
{
public:
    VanillaGradientDescent(MainWindow *mainWindow, SurfaceGraph *modifier);
    float run();

private:
    float computeCostFunction(float xHat, float zHat);
    float computeDfdx(float xHat, float zHat);
    float computeDfdz(float xHat, float zHat);
    SurfaceGraph *m_modifier;
    float m_xHat;
    float m_zHat;
    float m_dfdx;
    float m_dfdz;
    int m_nIterMax;
    float m_lr;
    float m_tol;
    float m_cost;
    QJSValue m_costEngine;
    QJSValue m_dfdxEngine;
    QJSValue m_dfdzEngine;
    MainWindow *m_mainWindow;
    Q3DSurface *m_graph;
    void plotPoint();
    QJSEngine m_engine;
};

#endif // VANILLAGRADIENTDESCENT_H
