#ifndef GLOBAL
#define GLOBAL
#include "cChain.h"
#include "cGraph.h"
#include "cIsotope.h"
#include "QFile"
#include "QTextStream"
#include <QCoreApplication>

extern cChain chain;
extern cGraph graph;

extern QFile OUT;
extern QTextStream sOUT;

extern QFile OUTMass;
extern QTextStream sOUTMass;

extern QTextStream Qcout;
extern int MaxTime;

#endif // GLOBAL

