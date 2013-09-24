#ifndef MyCell_h
#define MyCell_h

#include "TObject.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TCut.h"
#include "TVector3.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
//class that represents a single cell. For cell performance study within a given data sample (multiple runs)
//Yuxi Pan 10/19/2011
//_____________________________________________

class TTree;
class TH1F;
class TH2F;
class TGraph;
class TObjArray;
class Geom;
class CalibStr;
class MyCellRunDep;
class MyCellEvt;

class MyCell : public TObject {

public:
			MyCell();
			~MyCell();
			MyCell( const char* name , Int_t nstb, Int_t row, Int_t col, 
				CalibStr* gain, CalibStr* gcor, Geom* p_geom );		//name e.g.: Cellr0_c0_n1

	Int_t		GetNstb();							//starts from 1;
	Int_t		GetRow();							//starts from 0;
	Int_t		GetCol();							//startf from 0;
	Float_t		GetEta();
	Float_t		GetPhi();
	Float_t		GetCellWidthX();
	Float_t		GetCellWidthY();
	TTree*		GetPionSample( TCut cuts , char* runlist );			//assemble the pion sample from a set of runs		
	TTree*		GetPionSample( TCut cuts, Long_t runnum );
	MyCellRunDep*	GetMyCellrd( Long_t runnum );					//get MyCellRunDep object for run "runnum"
	TString		GetPartnerByXY( Float_t X2, Float_t Y2, Long_t runnum );	//Given photon impact point (X2,Y2), return the name 
											//and runnumber of the partner cell
	TString		GetPartnerByEtaPhi( Float_t Eta2, Float_t Phi2, Long_t runnum );
	TH1F*		GetAdcSpectrum( Long_t runnum );						
	TH1F*		GetLEDSpectrum( Long_t runnum );
	TH2F*		GetLEDvsEvt( Long_t runnum );						
	Int_t		GetNth( Long_t runnum, Float_t threshold );			//return # of triggers over threshold
	UChar_t		GetStatusBit( Long_t runnum );
	Float_t		GetGain();
	Float_t		GetGainCor();
	TObjArray*	GetRunDepArray() const { return VecMyCellRunDep; };
	const char*	GetName() const { return (const char*) mCellname; };
	void		Clear( Option_t* = "" );

	Bool_t		Legal( Int_t iew,Int_t nstb,Int_t row0,Int_t col0 );
	Int_t		SetNstb( Int_t nstb );
	Int_t		SetRow( Int_t row );
	Int_t		SetCol( Int_t col );
	Int_t		SetCellWidthXY( Float_t wx, Float_t wy );
	Int_t 		SetCellEtaPhi( Float_t eta, Float_t phi );
	Int_t		SetGain( CalibStr* gain );
	Int_t		SetGainCor( CalibStr* gcor );
	Int_t		SetGeom( Geom* geom );
	Int_t		SetStatusBit( Long_t runnum, UChar_t status );
	
	Int_t		InitMyCellRunDep( Long_t runnum );
	Int_t		InsertMyCellRunDep ( MyCellRunDep* mCellrun );

private:
	Int_t 		mNstb;
	Int_t		mRow;
	Int_t		mCol;
	Float_t		mWidthx;
	Float_t		mWidthy;
	Float_t		mEta;								//cell pseudorapidity
	Float_t		mPhi;
	Float_t		mGain;								//base gain
	Float_t		mGcor;								//current gain correction
	
	Geom*		mGeom;								//!
	TString		mCellname;						
	TGraph*		mIterhis;							//!
											//iteration history, m(rec) / m(pi)
	MyCellEvt*	mcevt;								//!
	
/*--------------These are the data members that need to go into MyCellRunDep object (run dependent object)----------------/
	UChar_t		status;
	TH1F*		adc;
	TH1F*		led;
	TH2F*		ledevt;								//LED adc vs event number
	TTree* pi;									//a set of pions with high E daughter photon 
											//being associated with this cell
*-----------------------------------------------------------------------------------------------------------------------*/

	TObjArray*	VecMyCellRunDep;						//->
											//an array that holds a list of MyCellRunDep objects,
											//each represents the performance of the current cell
											//within a run		
	
	ClassDef(MyCell,1)
};

#endif
