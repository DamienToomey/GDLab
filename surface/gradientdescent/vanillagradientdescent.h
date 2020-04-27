#ifndef VANILLAGRADIENTDESCENT_H
#define VANILLAGRADIENTDESCENT_H

#include "gradientdescent.h"

class VanillaGradientDescent : public GradientDescent
{
public:
    VanillaGradientDescent();
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
    virtual QColor color();
    virtual QString name();
    virtual bool curveIsDisplayed();
    virtual void setCurveIsDisplayed(bool curveIsDisplayed);
};

#endif // VANILLAGRADIENTDESCENT_H
