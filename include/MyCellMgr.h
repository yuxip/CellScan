#ifndef MyCellMgr_h
#define MyCellMgr_h

#include "TFile.h"
#include "TString.h"
#include "TObject.h"
#include "TSystem.h"
#include "TCut.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
//class that manages all the MyCell objects. Read Adc and OFile, populate each MyCell.
//Yuxi Pan 11/02/2011
//___________________________________________________________________

class MyCell;
class MyCellRunDep;
class MyCellEvt;
class TObjArray;
class CalibStr;
class Geom;
class TFile;
class FilesSet;
class TTree;

class MyCellMgr {

public:		
			MyCellMgr();
			MyCellMgr( Long_t runnum, const char* Outname, const char* option );		//a list of runs that need to be filled into MyCell objects
			~MyCellMgr();
	Int_t		InitFiles();
	Int_t		InitMyCells();									//initialize MyCellsArray
	Int_t		ReadADC( const char* adcdir, Bool_t write );					//populates the adc/LED spectrum for all MyCellRunDep object in all MyCell
	Int_t		ReadOutput( const char* ofdir, TCut cuts, Bool_t write );			//populates the pion sample
	Int_t 		SetCellStatus( Float_t LedBadTh, Int_t AveLEDRate, Bool_t write);
	Int_t		GetPointXY( Float_t eta, Float_t phi, Float_t& x, Float_t& y ,Int_t& nstb); 	//given eta/phi, calculate x, y coordinates and nstb
	MyCellRunDep*	GetPartnerByXY( Float_t x2, Float_t y2, Long_t runnum );
	Int_t		GetTrigRate( Long_t runnum, Int_t threshold, Float_t* AveRate, Float_t* RateSpread, Int_t (*TrigRate)[34][17], Bool_t write );					//calculate average trigger rate, and rate of each cell
	Int_t		InsertMyCell( MyCell* mycell );
	MyCell*		GetCell( const char* cname );			
	Int_t		WriteCellArray();
	Bool_t		Legal( Int_t iew, Int_t nstb, Int_t row0, Int_t col0 );
			//...under construction... Any function that manipulates multiple cells should be added in here	
	TObjArray*	VecMyCell;									//->
			
private:

	Int_t		InitRunDep(Long_t runnum);				//initialize
	TString 	mOutfile;
	TDirectory*	mDir;
//	TObjArray*	VecMyCell;
	CalibStr*	mGain;
	CalibStr*	mGcor;
	Geom*		mGeom;
//	ifstream*	mInFile;
	Long_t		mRun;				//it's very memory-consuming to write multiple MyCellRunDep objects together for each cell
	TFile*		fout;				//so I decided to make MyCells.root on a run-by-run basis
	ClassDef(MyCellMgr,1)

};

#endif
