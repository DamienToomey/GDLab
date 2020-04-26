#ifndef GRADIENTDESCENTWITHMOMENTUM_H
#define GRADIENTDESCENTWITHMOMENTUM_H

#include "gradientdescent.h"

class GradientDescentWithMomentum : public GradientDescent
{
public:
    GradientDescentWithMomentum(SurfaceGraph *modifier, QVector3D selectedPoint);
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
};

#endif // GRADIENTDESCENTWITHMOMENTUM_H
