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

private:
    double updateRule(double xHat, double dx, double lr);
};

#endif // VANILLAGRADIENTDESCENT_H
