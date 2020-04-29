#ifndef VANILLAGRADIENTDESCENT_H
#define VANILLAGRADIENTDESCENT_H

#include "gradientdescent.h"

class VanillaGradientDescent : public GradientDescent
{
public:
    VanillaGradientDescent();
    virtual vector<QVector3D> run();
    virtual QColor color();
    virtual QString name();
    virtual QList<QString> hyperParameters();

private:
    float updateRule(float xHat, float dfdx, float lr);
};

#endif // VANILLAGRADIENTDESCENT_H
