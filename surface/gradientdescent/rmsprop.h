#ifndef RMSPROP_H
#define RMSPROP_H

#include "gradientdescent.h"

class RMSProp : public GradientDescent
{
public:
    RMSProp();
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
    virtual QColor color();
    virtual QString name();
    float updateRule(float xHat, float dfdx, float lr, float decayRate, float& dxSquared);
};

#endif // RMSPROP_H
