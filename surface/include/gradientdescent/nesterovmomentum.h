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
    virtual bool hessianIsNecessary();

private:
    double updateRule(double xHat, double dx, double lr, double rho, double& vx);
};

#endif // NESTEROVMOMENTUM_H
