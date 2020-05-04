#ifndef NESTEROVMOMENTUM_H
#define NESTEROVMOMENTUM_H

#include "gradientdescent.h"

class NesterovMomentum : public GradientDescent
{
public:
    NesterovMomentum();
    virtual vector<QVector3D> run();
    virtual QColor color();
    virtual QString name();

private:
    float updateRule(float xHat, float dx, float lr, float rho, float& vx);
};

#endif // NESTEROVMOMENTUM_H
