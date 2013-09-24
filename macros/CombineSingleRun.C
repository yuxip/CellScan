#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "TKey.h"
using namespace::std;
  
  Int_t CombineSingleRun( Int_t runnum = 0 ,const char* ls = "large"){
	
	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	
	Char_t 		infilename[100];
	Char_t 		cn[30];
	Char_t		cellfile[100];
	Int_t 		runnum;
	MyCell* 	mcell 	= 0;
	MyCell* 	corcell = 0;
	MyCellRunDep* 	mcrd 	= 0;
	TFile*		fcell	= 0;
	Int_t 		n1 	= 0;
	Int_t		n2	= 0;
	
	if(!strcmp(ls,"large")){
		n1 = 1;
		n2 = 2;
	}
	else if(!strcmp(ls,"small")){
		n1 = 3;
		n2 = 4;
	}
	else{
		cout<<"invalid 2nd argument"<<endl;
		return -1;
	}

	for(Int_t nstb = n1; nstb <= n2; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){		//fix-up
			for(Int_t col0 = 0; col0 < 17; col0++){
				if(!Legal(2,nstb,row0,col0))continue;
				sprintf(cn,"Cellr%d_c%d_n%d",row0,col0,nstb);
				sprintf(cellfile,"/home/yuxip/FMS/CellScan/cells/%s.root",cn);
				fcell = new TFile(cellfile,"update");
				if(!fcell){
					cout<<cellfile<<" does not exist!!"<<endl;
					return -1;
				}
				if(!(mcell=(MyCell*)fcell->Get(cn))){
					cout<<"Combine.C: ERROR getting primary cell "<<cn<<endl;
					return -1;
				}
					
				
				sprintf(infilename,"/home/yuxip/FMS/CellScan/MyCellsNewBBCAsy_run%d.root",runnum);
				TFile* f1 = new TFile(infilename,"read");
				if(col0==0)cout<<"processing run: "<<runnum<<" for "<<cn<<endl;
				corcell = (MyCell*)f1->Get(cn);
				if(!corcell){
					cout<<"Combine.C: ERROR getting correspondence cell "<<cn<<endl;
					return -1;
				}
				mcrd = corcell->GetMyCellrd(runnum);
				mcell->InsertMyCellRunDep(mcrd);
	//			f1->Close();
				if(col0==0)mcell->GetRunDepArray()->Print();	//for test
				fcell->cd();
				mcell->Write("",TObject::kOverwrite);
				fcell->Close();
				mcrd  = 0;
				mcell = 0;
			}
		}
	}
	
	return 1;
  }
