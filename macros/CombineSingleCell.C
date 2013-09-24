#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "TKey.h"
using namespace::std;
//works on a particular cell, add MyCellRunDep object for all the runs from runlist  
  Int_t CombineSingleCell( const char* runlist = "/home/yuxip/FMS/CellScan/tmpfill15408.txt" ){
	
	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	
	TDirectory* t1 = gDirectory;	
	fstream inlist(runlist,ios::in);
	if(!inlist){
		cout<<runlist<<" does not exist!!"<<endl;
		return -1;
	}
	Char_t 		infilename[100];
	Char_t 		cn[30];
	Char_t		cellfile[100];
	Int_t 		runnum;
	MyCell* 	mcell 	= 0;
	MyCell* 	corcell = 0;
	MyCellRunDep* 	mcrd 	= 0;
	TFile*		fcell	= 0;
	
	Int_t nstb = 4;
	Int_t col0 = 4;
	Int_t row0 = 19;

	if(!Legal(2,nstb,row0,col0)){
		cout<<"illegal (nstb,row0,col0). Abort!!"<<endl;
		return -1;
	}
	sprintf(cn,"Cellr%d_c%d_n%d",row0,col0,nstb);
	sprintf(cellfile,"/home/yuxip/FMS/CellScan/cells/tmp%s.root",cn);
	fcell = new TFile(cellfile,"update");
	if(!fcell){
		cout<<cellfile<<" does not exist!!"<<endl;
		return -1;
	}
	if(!(mcell=(MyCell*)fcell->Get(cn))){
		cout<<"Combine.C: ERROR getting primary cell "<<cn<<endl;
		return -2;
	}
					
	while(inlist>>runnum){
		sprintf(infilename,"/home/yuxip/FMS/CellScan/MyCellsNewBBCAsy_run%d.root",runnum);
		TFile* f1 = new TFile(infilename,"read");
		if(col0==0)cout<<"processing run: "<<runnum<<endl;
		corcell = (MyCell*)f1->Get(cn);
		if(!corcell){
			cout<<"Combine.C: ERROR getting correspondence cell "<<cn<<endl;
			return -1;
		}
		mcrd = corcell->GetMyCellrd(runnum);
		mcell->InsertMyCellRunDep(mcrd);
		f1->Close();
		
	}
//	inlist.clear();
//	inlist.seekg(0,ios::beg);
		
	mcell->GetRunDepArray()->Print();	//for test
	
	fcell->cd();
	mcell->Write("",TObject::kOverwrite);
	fcell->Close();
	mcrd  = 0;
	mcell = 0;
	
	return 1;	
  }
