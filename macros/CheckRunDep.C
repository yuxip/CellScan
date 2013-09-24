#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "TKey.h"
using namespace::std;
  
  Int_t CheckRunDep( const char* runlist = "/home/yuxip/FMS/CellScan/AllMgrRun.list" ,const char* ls = "small"){
	
	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	
	TDirectory* t1 = gDirectory;	
	fstream inlist(runlist,ios::in);
	fstream report("checkRunDep_report_n4",ios::out);
	if(!inlist){
		cout<<runlist<<" does not exist!!"<<endl;
		return -1;
	}
	Char_t 		infilename[100];
	Char_t 		cn[30];
	Char_t		cellfile[100];
	Int_t 		runnum;
	MyCell* 	mcell 	= 0;
//	MyCell* 	corcell = 0;
	MyCellRunDep* 	mcrd 	= 0;
	TFile*		fcell	= 0;
	Int_t 		n1 	= 0;
	Int_t		n2	= 0;
	
	if(!strcmp(ls,"large")){
		n1 = 2;
		n2 = 2;
	}
	else if(!strcmp(ls,"small")){
		n1 = 4;
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
				if(col0==0)cout<<"processing "<<cn<<endl;
				sprintf(cellfile,"/home/yuxip/FMS/CellScan/cells/%s.root",cn);
				fcell = new TFile(cellfile,"read");
				if(!fcell){
					cout<<cellfile<<" does not exist!!"<<endl;
					report<<cn<<"  file does not exist"<<endl;
					continue;
//					return -1;
				}
//				t1->cd();
				if(!(mcell=(MyCell*)fcell->Get(cn))){
					cout<<"CheckRunDep.C: ERROR getting primary cell "<<cn<<endl;
					report<<cn<<" MyCell object does not exist"<<endl;
					continue;
//					return -1;
				}
					
				while(inlist>>runnum){
//					sprintf(infilename,"/home/yuxip/FMS/CellScan/MyCells_run%d.root",runnum);
//					TFile* f1 = new TFile(infilename,"read");
//					if(col0==0)cout<<"processing run: "<<runnum<<endl;
//					corcell = (MyCell*)f1->Get(cn);
//					if(!corcell){
//						cout<<"Combine.C: ERROR getting correspondence cell "<<cn<<endl;
//						return -1;
//					}
					mcrd = mcell->GetMyCellrd(runnum);
					if(!mcrd){
						cout<<"MyCellRunDep object of "<<cn<<" at run "<<runnum<<"does not exist"<<endl;
						report<<"MyCellRunDep object of "<<cn<<" at run "<<runnum<<"does not exist"<<endl;
						continue;
					}
					
				}
				inlist.clear();
				inlist.seekg(0,ios::beg);
					
//				if(col0==0)mcell->GetRunDepArray()->Print();	//for test
				
//				fcell->cd();
//				mcell->Write("",TObject::kOverwrite);
				fcell->Close();
				mcrd  = 0;
				mcell = 0;
			}
		}
	}
	

	return 1;
  }
