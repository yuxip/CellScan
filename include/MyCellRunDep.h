#ifndef MyCellRunDep_h
#define MyCellRunDep_h

#include "TObject.h"
#include "TFile.h"
#include "TString.h"
#include "TH1F.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
using namespace std;
//class that represents a single cell dynamic performance within a run
//Yuxi Pan 11/01/2011
//___________________________________________________________________

class TTree;
class TH1F;
class TH2F;
class MyCellEvt;										//Pion sample Tree structure
class TCut;

class MyCellRunDep : public TObject {

public:
				MyCellRunDep();
				~MyCellRunDep();
				MyCellRunDep( const char* name );
		TTree*		GetPionSample( TCut cuts );
		TH1F*		GetAdcSpectrum();
		TH1F*		GetLEDSpectrum();
		TH2F*		GetLEDvsEvt();
//		MyCellRunDep*	GetPartnerByXY( Float_t X2, Float_t Y2 );		//Given photon impact point (X2,Y2), Find the partner cell
//		MyCellRunDep*	GetPartnerByEtaPhi( Float_t Eta2, Float_t Phi2 );
		Int_t		GetRunNumber();
		UChar_t		GetStatusBit();
		MyCellEvt*	GetMyCellEvt();
		const char*	GetName() const { return (const char*) mCellrdname; };

		Int_t		SetMyCellEvt( MyCellEvt* mycevt );
		Int_t		SetName( const char* name );
		Int_t		SetStatusBit( UChar_t status );
		Int_t		SetPionSample(TTree* Trpi);
		Int_t		InitTree( MyCellEvt* mycevt );
		void		Clear( Option_t* option = "" );

private:
		TString		mCellrdname;						
		Int_t		mNstb;							
		Int_t		mRow;							
		Int_t		mCol;							
		Long_t		mRun;							
		TH1F*		mAdc;							//->
		TH1F*		mLed;							//->
		TH2F*		mLedEvt;						//->
		TTree*		mMyCellTr;						//->
		UChar_t		mStatus;						
		MyCellEvt*	mMyCevt;						//!			
	
		ClassDef(MyCellRunDep,1)

};

#endif
