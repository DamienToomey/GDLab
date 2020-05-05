## Add another gradient descent method to GDLab

1. In `include/gradientdescent`, create `mygradientdescent.h`
2. In `src/gradientdescent`, create `mygradientdescent.cpp`
3. If your gradient descent method does NOT require the hessian,
  a. copy the content of `adagrad.h` to `mygradientdescent.h`
  b. copy the content of `adagrad.cpp` to `mygradientdescent.cpp`
4. If your gradient descent method requires the hessian,
  a. copy the content of `newtonregularized.h` to `mygradientdescent.h`
  b. copy the content of `newtonregularized.cpp` to `mygradientdescent.cpp`
5. adapt `mygradientdescent.h` and `mygradientdescent.cpp` to your gradient descent method by changing the update rule for example
6. In `include/mainwindow.h`, add `#include "gradientdescent/mygradientdescent.h"`
7. In `include/mainwindow.cpp`, add `m_gradientDescentMethodToGradientDescent["MyGradientDescentMethod"] = new MyGradientDescentMethod();` to the constructor of the class

You're good to go!

## Not working?

Open an issue :)