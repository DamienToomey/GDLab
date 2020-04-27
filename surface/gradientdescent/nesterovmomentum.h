#ifndef NESTEROVMOMENTUM_H
#define NESTEROVMOMENTUM_H

#include "gradientdescent.h"

class NesterovMomentum : public GradientDescent
{
public:
    NesterovMomentum();
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
    virtual QColor color();
    virtual QString name();
    virtual bool curveIsDisplayed();
    virtual void setCurveIsDisplayed(bool curveIsDisplayed);
};

#endif // NESTEROVMOMENTUM_H
