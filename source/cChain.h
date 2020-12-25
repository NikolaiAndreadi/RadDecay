#ifndef CCHAIN_H
#define CCHAIN_H

#include <QVector>
#include "cIsotope.h"

class cChain
{
    int iIsotopeCounter;
    QVector <cIsotope> vecChain;

    void FindDaughterIsotope(short Channel, int fromID, QString &Name, short int &tIsotopeMass);
    bool AtSystem (QString s_ElementName, short i_IsotopeMass);


public:

    cChain()
    {iIsotopeCounter = -1;}

    int AddIsotope (QString s_ElementName, short i_IsotopeMass, float f_Mass);
    void Debug_ShowInfo();
    void Debug_QuickInfo();
    void CalcChain();
    cIsotope* GetElement (int ID);
    inline int GetIsotopeCount () {return iIsotopeCounter;}
};

#endif // CCHAIN_H
