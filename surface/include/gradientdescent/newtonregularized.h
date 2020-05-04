#ifndef NEWTON_H
#define NEWTON_H

#include "gradientdescent.h"
#include <eigen-3.3.7/Eigen/Eigenvalues>

using namespace Eigen;

class NewtonRegularized : public GradientDescent
{
public:
    NewtonRegularized();
    virtual vector<QVector3D> run();
    virtual QColor color();
    virtual QString name();
    virtual bool hessianIsNecessary();

private:
    Vector2d updateRule(Vector2d wHat, Vector2d g, Matrix2d H,  Matrix2d I, float lamb);
};

#endif // NEWTON_H
