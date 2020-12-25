#ifndef ODESOLVER
#define ODESOLVER

#include "global.h"
#include "boost/numeric/odeint.hpp"

extern void ODE_SOLV(double init, double end, double step, double ABS, double REL, QTextStream& stream1, QTextStream &stream2);

#endif // ODESOLVER

