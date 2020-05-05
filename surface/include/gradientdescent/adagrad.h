#ifndef ADAGRAD_H
#define ADAGRAD_H

#include "gradientdescent.h"

class AdaGrad : public GradientDescent
{
public:
    AdaGrad();
    virtual vector<QVector3D> run();
    virtual QColor color();
    virtual QString name();
    virtual bool hessianIsNecessary();

private:
    double updateRule(double xHat, double dx, double lr, double& dxSquared);
};

#endif // ADAGRAD_H
