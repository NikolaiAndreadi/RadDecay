#include "cChain.h"
#include <QSettings>
#include <QDebug>
#include "global.h"

int cChain::AddIsotope(QString s_ElementName, short i_IsotopeMass, float f_Mass)
{
    cIsotope temp;
    temp.FillData(s_ElementName,i_IsotopeMass,f_Mass);
    temp.SetID(++iIsotopeCounter);
    vecChain.append(temp);
    //vecChain[iIsotopeCounter].Debug_ShowInfo();
    return temp.GetID();
}

void cChain::FindDaughterIsotope(short Channel, int fromID, QString &Name, short &tIsotopeMass)
{
    cIsotope temp = vecChain[fromID];

    short tempZ,tempA;

    if (temp.ChanMode(Channel) == "a")
    {
        tempZ = temp.GetIsotopeCharge()-2;
        tempA = temp.GetIsotopeMass()-4;
    }
    else if (temp.ChanMode(Channel) == "b-")
    {
        tempZ = temp.GetIsotopeCharge()+1;
        tempA = temp.GetIsotopeMass();
    }
    else if (temp.ChanMode(Channel) == "b+")
    {
        tempZ = temp.GetIsotopeCharge()-1;
        tempA = temp.GetIsotopeMass();
    }

    QString path = QCoreApplication::applicationDirPath() + "/db/elements.ini";
    QSettings Elements (path, QSettings::IniFormat);
    QString DElement;

    Elements.beginGroup("Elements");
            DElement = Elements.value (QString::number(tempZ),"").toString();
    Elements.endGroup();

        Name         = DElement;
        tIsotopeMass = tempA;
}

bool cChain::AtSystem (QString s_ElementName, short i_IsotopeMass)
{
    for (int i = 0; i <=iIsotopeCounter; i++)
        if ((vecChain[i].GetIsotopeName() == s_ElementName) && (vecChain[i].GetIsotopeMass() == i_IsotopeMass))
            return true;
    return false;
}

void cChain::CalcChain()
{
    QString tempName;
    short   tempIsotopeMass;
    int iDaughterID;

    for (int n = 0; n <=iIsotopeCounter; n++)
        for (int i = 1; i <= vecChain[n].GetNumberOfChan(); i++)
        {
            FindDaughterIsotope(i,n,tempName,tempIsotopeMass);
            if (!AtSystem(tempName,tempIsotopeMass))
                iDaughterID = AddIsotope(tempName,tempIsotopeMass,0.0f);
            graph.SetLength (iIsotopeCounter+1);
            graph.SetValue(vecChain[n].ChanProbability(i),vecChain[n].GetID(),iDaughterID);
        }
}
cIsotope* cChain::GetElement (int ID)
{
    return &vecChain[ID];
}

void cChain::Debug_ShowInfo()
{
    qDebug () << "cChain Debug_ShowInfo";
    qDebug() << ".....";
    qDebug () << "MaxID: " << iIsotopeCounter;
    qDebug() << "#########################";
    for (int i = 0; i <= iIsotopeCounter; i++)
    {
        qDebug () << "Element #" << i << ": " << vecChain[i].GetIsotopeMass() <<  vecChain[i].GetIsotopeName();
        qDebug () << "lambda = " << vecChain[i].GetLambda();
        qDebug() << ".....";
        for (int j = 1; j <= vecChain[i].GetNumberOfChan(); j++)
        {
            qDebug() << "Channel "      << j;
            qDebug() << "Mode: "        << vecChain[i].ChanMode(j);
            qDebug() << "Probability: " << vecChain[i].ChanProbability(j);
        }
        qDebug() << "#########################";
    }
    qDebug() << "==========================================";
}

void cChain::Debug_QuickInfo()
{
    qDebug () << "cChain Debug_QuickInfo";
    qDebug () << "MaxID: " << iIsotopeCounter;
    for (int i = 0; i <= iIsotopeCounter; i++)
        qDebug () << "Element #" << i << ": " << vecChain[i].GetIsotopeMass() <<  vecChain[i].GetIsotopeName();
}
