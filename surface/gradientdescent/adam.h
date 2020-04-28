#ifndef ADAM_H
#define ADAM_H

#include "gradientdescent.h"

class Adam : public GradientDescent
{
public:
    Adam();
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
    virtual QColor color();
    virtual QString name();

private:
    float updateRule(float xHat, float dfdx, float lr, float beta1, float beta2,
                     float& firstMoment, float& secondMoment, int k);

};

#endif // ADAM_H
