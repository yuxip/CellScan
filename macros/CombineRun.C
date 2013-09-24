#include <fstream>
#include "TKey.h"
using namespace::std;
  
  Int_t CombineRun( const char* runlist = "/home/yuxip/FMS/CellScan/MgrRun.list" ){
	
	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	
	const Char_t* outfilename = "/home/yuxip/FMS/CellScan/MyCellsNewBBCAsy_Fill15419_4.root";
//	TFile* fout = new TFile(outfilename,"recreate");
	MyCellMgr* cmgr = new MyCellMgr(0,outfilename,"recreate");
	TDirectory* t1 = gDirectory;	
	fstream inlist(runlist,ios::in);
	if(!inlist){
		cout<<runlist<<" does not exist!!"<<endl;
		return -1;
	}
	Char_t 		infilename[100];
	Char_t 		cn[30];
	Int_t 		runnum;
	MyCell* 	mcell 	= 0;
	MyCell* 	corcell = 0;
	MyCellRunDep* 	mcrd 	= 0;

	while(inlist>>runnum){
		sprintf(infilename,"/home/yuxip/FMS/CellScan/MyCellsNewBBCAsy_run%d.root",runnum);
		TFile* f1 = new TFile(infilename,"read");
		cout<<"processing run: "<<runnum<<endl;

		for(Int_t nstb = 1; nstb <= 4; nstb++){
                	for(Int_t row0 = 0; row0 < 34; row0++){
                        	for(Int_t col0 = 0; col0 < 17; col0++){
                                	if(!Legal(2,nstb,row0,col0))continue;
				
					sprintf(cn,"Cellr%d_c%d_n%d",row0,col0,nstb);
					t1->cd();
					if(!(mcell=(MyCell*)cmgr->GetCell(cn))){
						cout<<"Combine.C: ERROR getting primary cell "<<cn<<endl;
						return -1;
					}
					//mcell->Print();
					corcell = (MyCell*)f1->Get(cn);
					if(!corcell){
						cout<<"Combine.C: ERROR getting correspondence cell "<<cn<<endl;
						return -1;
					}
					mcrd = corcell->GetMyCellrd(runnum);
					mcell->InsertMyCellRunDep(mcrd);
					if(col0==0)mcell->GetRunDepArray()->Print();	//for test
					mcrd  = 0;
					mcell = 0;
				}
			}
		}
	}
	cmgr->WriteCellArray();

	return 1;
  }
