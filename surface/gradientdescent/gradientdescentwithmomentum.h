#ifndef GRADIENTDESCENTWITHMOMENTUM_H
#define GRADIENTDESCENTWITHMOMENTUM_H

#include "gradientdescent.h"

class GradientDescentWithMomentum : public GradientDescent
{
public:
    GradientDescentWithMomentum();
    virtual vector<QVector3D> run(float lr, float tol, int nIterMax);
    virtual QColor color();
    virtual QString name();
    virtual bool curveIsDisplayed();
    virtual void setCurveIsDisplayed(bool curveIsDisplayed);

};

#endif // GRADIENTDESCENTWITHMOMENTUM_H
