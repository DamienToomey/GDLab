#ifndef NESTEROVMOMENTUM_H
#define NESTEROVMOMENTUM_H

#include "gradientdescent.h"

class NesterovMomentum : public GradientDescent
{
public:
    NesterovMomentum(SurfaceGraph *modifier, QVector3D selectedPoint);
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
};

#endif // NESTEROVMOMENTUM_H
