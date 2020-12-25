#include <QCoreApplication>
#include "global.h"
#include "odesolver.h"
#include <QSettings>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString path = QCoreApplication::applicationDirPath() + "/input/input.dat";
    QSettings Data (path, QSettings::IniFormat);
    Data.beginGroup("FirstElemData");
        QString sElem   = Data.value  ("Elem","").toString();
        int     iMass   = Data.value  ("Mass","").toInt();
        float   fWeight = Data.value  ("Weight","").toFloat();
    Data.endGroup();

    chain.AddIsotope(sElem, iMass, fWeight);
    chain.CalcChain();

    QString outpath = QCoreApplication::applicationDirPath() + "/output/out.dat";
    QString outpathmass = QCoreApplication::applicationDirPath() + "/output/outmass.dat";

    QFile OUT(outpath);
    QTextStream sOUT (&OUT);

    QFile OUTMass(outpathmass);
    QTextStream sOUTMass (&OUTMass);

    OUT.open (QIODevice::WriteOnly);
    OUTMass.open (QIODevice::WriteOnly);

    sOUT << "time" << '\t';
    sOUTMass << "time" << '\t';

    for (unsigned int i = 0; i < graph.GetLength(); i++)
    {
        cIsotope *pEl = chain.GetElement(i);
        sOUT << pEl->GetIsotopeMass() << pEl->GetIsotopeName() << '\t';
        sOUTMass << pEl->GetIsotopeMass() << pEl->GetIsotopeName() << '\t';
    }
    sOUT << endl;
    sOUTMass << endl;

    graph.ShowDecayChain();

    Data.beginGroup("OdeSolvData");
        int iInitTime = Data.value ("Inittime","").toInt();
        int iStopTime = Data.value ("Stoptime","").toInt();
        int iStep     = Data.value ("Step","").toInt();
        float fAbs    = Data.value ("ABSERR","").toFloat();
        float fRel    = Data.value ("RELERR","").toFloat();
    Data.endGroup();

    MaxTime = iStopTime;

    ODE_SOLV(iInitTime, iStopTime , iStep ,fAbs, fRel, sOUT,sOUTMass);

    OUT.close();
    OUTMass.close();

    return a.exec();
}

