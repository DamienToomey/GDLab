#ifndef RMSPROP_H
#define RMSPROP_H

#include "gradientdescent.h"

class RMSProp : public GradientDescent
{
public:
    RMSProp();
    virtual vector<QVector3D> run();
    virtual QColor color();
    virtual QString name();

private:
    float updateRule(float xHat, float dx, float lr, float decayRate, float& dxSquared);
};

#endif // RMSPROP_H
