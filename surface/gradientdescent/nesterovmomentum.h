#ifndef NESTEROVMOMENTUM_H
#define NESTEROVMOMENTUM_H

#include "gradientdescent.h"

class NesterovMomentum : public GradientDescent
{
public:
    NesterovMomentum();
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
    virtual QColor color();
    virtual QString name();
    float updateRule(float xHat, float dfdx, float lr, float rho, float& vx);
};

#endif // NESTEROVMOMENTUM_H
