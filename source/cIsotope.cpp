#include "cIsotope.h"
#include <QDebug>
#include <qmath.h>
#include <QSettings>
#include <QStringList>
#include <QCoreApplication>

//====================
//=====INI FILE ======
//====================
//[Th 232]           +
//Z = 90             +
//ME = 35448.307     +
//HL = "1.405E10 y"  +
//CHAN=1             +
//mode1 = "a"        +
//P1  = 1            +
//E1  = "4.0828 MeV" +
//====================


void cIsotope::FillData (QString s_ElementName, short i_IsotopeMass, float f_Mass)
{
	fMass         = f_Mass;
	iIsotopeMass  = i_IsotopeMass;
    sElementName = s_ElementName;

    QString path = QCoreApplication::applicationDirPath() + "/db/isotopes.ini";
	
    QSettings Data (path, QSettings::IniFormat);
	QString Search = sElementName + ' ' + QString::number(iIsotopeMass);
	
	Data.beginGroup(Search);
	
		iElementNumber = Data.value  ("Z","").toInt();
		fMassExcess    = Data.value  ("ME","").toFloat();
		sHalfLife      = Data.value  ("HL","").toString();
		
/////////////////////////	
//Calculating Half-Life//
/////////////////////////
        if ( !QString::compare(sHalfLife,"stable", Qt::CaseInsensitive) )
		{
			bStable = true;
			fLambda = 0.0;
		}
		else
		{
			bStable = false;
			//string -> float
			QStringList HLTranslater =	sHalfLife.split(" ",QString::SkipEmptyParts);

            QString HLSplitted = HLTranslater.at(0);
            QString HLScale    = HLTranslater.at(1);

			float fHalfLife;
			
			if (!QString::compare(HLScale,"y",Qt::CaseInsensitive) )
            fHalfLife = HLSplitted.toFloat()*31557600.0f;
            if (!QString::compare(HLScale,"d",Qt::CaseInsensitive) )
            fHalfLife = HLSplitted.toFloat()*86400.0f;
            if (!QString::compare(HLScale,"h",Qt::CaseInsensitive) )
            fHalfLife = HLSplitted.toFloat()*3600.0f;
            if (!QString::compare(HLScale,"m",Qt::CaseInsensitive) )
            fHalfLife = HLSplitted.toFloat()*60.0f;
            if (!QString::compare(HLScale,"s",Qt::CaseInsensitive) )
            fHalfLife = HLSplitted.toFloat();
            if (!QString::compare(HLScale,"ms",Qt::CaseInsensitive))
            fHalfLife = HLSplitted.toFloat()*0.001f;
            if (!QString::compare(HLScale,"us",Qt::CaseInsensitive) )
            fHalfLife = HLSplitted.toFloat()*0.000001f;
            if (!QString::compare(HLScale,"ns",Qt::CaseInsensitive) )
            fHalfLife = HLSplitted.toFloat()*0.000000001f;
/////////////////////////////
//END Calculating Half-Life//
/////////////////////////////
			
            fLambda = qLn (2.0)/fHalfLife;
            fParticleMass = (PROTONMASS*iElementNumber + (iIsotopeMass-iElementNumber)*NEUTRONMASS - fMassExcess)*1.782661907e-27;//in gramm
		}

/////////////////////////	
//Reading Channels///////
/////////////////////////
		
        if (!bStable)
		{
			QString sMode,sP,sE, sCalcE;
			iChanNumber = Data.value ("CHAN","").toInt();
			
            for (int i=1; i <= iChanNumber; i++)
            {
				sMode = "mode"+QString::number(i);
				sP = "P"+QString::number(i);
                sE = "E"+QString::number(i);
				
				tChan[i].mode =  Data.value(sMode,"").toString();
                tChan[i].P    =  Data.value(sP,"").toDouble();
				
                sCalcE        =  Data.value(sE,"").toString();
				// translating MeV -> keV
                QStringList ECalc = sCalcE.split(" ",QString::SkipEmptyParts);
                QString sCalcEvalue = ECalc.at(0);
                tChan[i].E = sCalcEvalue.toFloat();
                if (!QString::compare(ECalc.at(1),"MeV",Qt::CaseInsensitive))
                    tChan[i].E *= 1000.0f;
            }
		}
        else        iChanNumber = 0;
				
/////////////////////////	
//END Reading Channels///
/////////////////////////		
	
	Data.endGroup();
	
	ID = 0;
}




void cIsotope::Debug_ShowInfo ()
{
    qDebug () << "cIsotope Debug_ShowInfo";
    qDebug() << "Name" << GetIsotopeName();
    qDebug() << "Mass" << GetIsotopeMass();
    qDebug() << "Charge" << GetIsotopeCharge();
    qDebug() << "NumberOfChan" << GetNumberOfChan();
    qDebug() << "Is stable" << Stability();
    qDebug() << "Half-life" << GetHL();
    qDebug() << "Lambda" << GetLambda();

    for (int i = 1; i <= GetNumberOfChan(); i++)
    {
        qDebug() << "Channel "      << i;
        qDebug() << "Mode: "        << ChanMode(i);
        qDebug() << "Energy, keV: " << ChanEnergy(i);
        qDebug() << "Probability: " << ChanProbability(i);
    }
    qDebug() << "==========================================";
}
