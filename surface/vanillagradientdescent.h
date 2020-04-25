#ifndef VANILLAGRADIENTDESCENT_H
#define VANILLAGRADIENTDESCENT_H

#include "gradientdescent.h"

class VanillaGradientDescent : public GradientDescent
{
public:
    VanillaGradientDescent(MainWindow *mainWindow);
    virtual float run();
};

#endif // VANILLAGRADIENTDESCENT_H
