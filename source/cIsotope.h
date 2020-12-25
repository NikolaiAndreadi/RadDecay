#ifndef CISOTOPE_H
    #define CISOTOPE_H

#include <QString>

// TChan - struct for defenition of decay channels
struct TChan
{
    QString mode;   // alfa, beta-, beta+ decay
    double  P;      // probability of decay
    double  E;      // energy of decay
};

class cIsotope
{
        static const int MAXCHAN = 3;	// Max number of channels of decay
        const float PROTONMASS = 938.2720813; //MeV
        const float NEUTRONMASS = 939.5654133; //MeV
		
		QString sElementName;	// Element name                            +
		short   iIsotopeMass;	// Isotope mass number                     +
																		   
		short   iElementNumber; // Element charge number                   +
		float   fMassExcess;    // Mass excess, for calculating molar mass +
		
		TChan   tChan[MAXCHAN]; // Channel array						   +
		short   iChanNumber;    // Number of decay						   +
		

		float   fMass;          // mass of sample, g                       +
        double   fParticleMass;
		bool    bStable;        // true - stable isotope				   +
		double  fLambda;        // Decay constant						   +
		QString sHalfLife;      // Half-life							   +
		
		int ID;                 // ID of each element		
		
		
	public:
		
		// Fill data - read isotope data frop isotopes.ini
		// Look for definition in cIsotope.cpp
		void FillData (QString s_ElementName, short i_IsotopeMass, float f_Mass);
		
		// Show all isotope data in console
		void Debug_ShowInfo ();
		
		//Setting data
        inline void SetID (int i_ID) {ID = i_ID;}
        inline int  GetID ()         {return ID;}
		
		//Reading data
		inline QString GetIsotopeName ()  {return sElementName;}
		inline short   GetIsotopeMass ()  {return iIsotopeMass;}
		inline short   GetIsotopeCharge() {return iElementNumber;}
		inline short   GetNumberOfChan () {return iChanNumber;}
		inline bool    Stability ()       {return bStable;}
		inline QString GetHL ()           {return sHalfLife;}
		inline double  GetLambda ()       {return fLambda;}
        inline double  GetMass()          {return fMass;}
        inline double  GetParticleMass()  {return fParticleMass;}
		
		
        inline QString ChanMode     (short channel) {return tChan[channel].mode;}
		inline double  ChanEnergy      (short channel) {return tChan[channel].E;}
		inline double  ChanProbability (short channel) {return tChan[channel].P;}
		
		
};

#endif
// CISOTOPE_H
