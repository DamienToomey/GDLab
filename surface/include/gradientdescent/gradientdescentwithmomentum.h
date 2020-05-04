#ifndef GRADIENTDESCENTWITHMOMENTUM_H
#define GRADIENTDESCENTWITHMOMENTUM_H

#include "gradientdescent.h"

class GradientDescentWithMomentum : public GradientDescent
{
public:
    GradientDescentWithMomentum();
    virtual vector<QVector3D> run();
    virtual QColor color();
    virtual QString name();

private:
    float updateRule(float xHat, float dx, float lr, float rho, float& vx);
};

#endif // GRADIENTDESCENTWITHMOMENTUM_H
