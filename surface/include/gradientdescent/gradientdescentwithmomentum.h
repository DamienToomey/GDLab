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
    double updateRule(double xHat, double dx, double lr, double rho, double& vx);
};

#endif // GRADIENTDESCENTWITHMOMENTUM_H
