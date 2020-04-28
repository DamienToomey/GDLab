#ifndef ADAGRAD_H
#define ADAGRAD_H

#include "gradientdescent.h"

class AdaGrad : public GradientDescent
{
public:
    AdaGrad();
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
    virtual QColor color();
    virtual QString name();
    float updateRule(float xHat, float dfdx, float lr, float& dxSquared);
};

#endif // ADAGRAD_H
