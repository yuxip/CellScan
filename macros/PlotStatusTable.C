#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace::std;

  Int_t PlotStatusTable( Long_t runnum = 12098007, const char* sl = "large" ){
	
	gStyle->SetOptStat(0);
	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");

	Char_t outfilename[50];
	sprintf(outfilename,"%sCellStatus_run%ld_n1.ps",sl,runnum);
	TCanvas* c1 = new TCanvas("c1","c1",1100,700);
	Int_t ch 	= 0;
	Int_t n1 	= 0;
	Int_t n2 	= 0;
	Int_t ncol 	= 0;
	if(!strcmp(sl,"large")){
		cout<<"generating status table for large cells"<<endl;
		TH2F* f2 = new TH2F("f2","f2",394,0,394,8,0,8);
		ch = 394;
		n1 = 1;
		n2 = 1;
		ncol = 17;
	}
	else if(!strcmp(sl,"small")){
		cout<<"generating status table for small cells"<<endl;
		TH2F* f2 = new TH2F("f2","f2",238,0,238,8,0,8);
		ch = 238;
		n1 = 4;
		n2 = 4;
		ncol = 12;
	}
	else{
		cout<<"invalid 2nd argument"<<endl;
		return -1;
	}
	
	f2->SetTitle("Cell Status, nstb1");
	
	Int_t 	ich = 0;
	Char_t 	cn[30];
	Char_t 	cellfile[100];
	TFile* 	fcell = 0;
	MyCell* mcell = 0;
	Int_t	ibit = 0;
	UChar_t	status = 0x00;
	for(Int_t nstb = n1; nstb <=n2; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){
			for(Int_t col0 = 0; col0 < 17; col0++){

				if(!Legal(2,nstb,row0,col0))continue;
				ich = row0*ncol + col0;
				if(col0==0)cout<<"processing channel "<<ich<<endl;
				sprintf(cn,"Cellr%d_c%d_n%d",row0,col0,nstb);
				sprintf(cellfile,"/home/yuxip/FMS/CellScan/cells/%s.root",cn);
				fcell = new TFile(cellfile,"read");
				if(!fcell){
					cout<<"file "<<cellfile<<" does not exist!!"<<endl;
					return -1;
				}
				mcell = (MyCell*)fcell->Get(cn);
				if(!mcell){
					cout<<"ERROR getting object "<<cn<<endl;
					return -1;
				}
				mcell->Print();
				status = mcell->GetStatusBit(runnum);
				if(col0==0)cout<<"status: 0x"<<hex<<(Int_t)status<<endl;
				cout<<dec<<endl;
				for(Int_t i = 0; i < 8; i++){
					ibit = Int_t((status>>i)&0x01);
					if(col0==0)cout<<cn<<" bit"<<i<<": "<<ibit<<endl;
					if(i>=5&&ibit==1){
						f2->SetBinContent(ich+1,i+1,ibit+1);
					}
					if(i<5&&ibit==1){
						f2->SetBinContent(ich+1,i+1,ibit);
					}
				}
				
				delete fcell;
				fcell = 0;
			}
		}
	}
	cout<<"output file: "<<outfilename<<endl;
	f2->Draw("COLZ");
	c1->Print(outfilename);
	
	return 1;
  }
				
				




							









