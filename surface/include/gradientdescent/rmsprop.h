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
    double updateRule(double xHat, double dx, double lr, double decayRate, double& dxSquared);
};

#endif // RMSPROP_H
