#ifndef VANILLAGRADIENTDESCENT_H
#define VANILLAGRADIENTDESCENT_H

#include "gradientdescent.h"

class VanillaGradientDescent : public GradientDescent
{
public:
    VanillaGradientDescent(SurfaceGraph *modifier, QVector3D selectedPoint);
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
};

#endif // VANILLAGRADIENTDESCENT_H
