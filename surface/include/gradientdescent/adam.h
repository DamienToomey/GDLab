#ifndef ADAM_H
#define ADAM_H

#include "gradientdescent.h"

class Adam : public GradientDescent
{
public:
    Adam();
    virtual vector<QVector3D> run();
    virtual QColor color();
    virtual QString name();
    virtual bool hessianIsNecessary();

private:
    double updateRule(double xHat, double dx, double lr, double beta1, double beta2,
                      double& firstMoment, double& secondMoment, int k);

};

#endif // ADAM_H
