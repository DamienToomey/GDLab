#ifndef GRADIENTDESCENTWITHMOMENTUM_H
#define GRADIENTDESCENTWITHMOMENTUM_H

#include "gradientdescent.h"

class GradientDescentWithMomentum : public GradientDescent
{
public:
    GradientDescentWithMomentum();
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
    virtual QColor color();
    virtual QString name();
    float updateRule(float xHat, float dfdx, float lr, float rho, float& vx);

};

#endif // GRADIENTDESCENTWITHMOMENTUM_H
