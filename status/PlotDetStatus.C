#include<iostream>
#include<fstream>
using namespace std;
//========================Plot cell status on detector layout=======//
//

  Int_t PlotDetStatus( Int_t runnum = 12098007 ){
	
	gROOT->LoadMacro("ConvertTH2Bin.C");
	gROOT->LoadMacro("Legal.C");
	TCanvas* c1 = new TCanvas("c1","c1",600,400);
	Char_t	title[30];
	sprintf(title,"CellStatus_run%d",runnum);
	Char_t outfilename[50];
	sprintf(outfilename,"DetStatus_run%d.ps",runnum);	

	Int_t	ew;
	Int_t	nstb;
	Int_t	ch;
	Int_t	nstb;
	Int_t	row0;
	Int_t	col0;
	Char_t	lstatus[4];
	UChar_t	status;

	TH2F* slarge	= new TH2F("slarge",title,34,-98.6,98.6,34,-98.6,98.6);
	TH2F* ssmall	= new TH2F("ssmall","ssmall",52,-98.6,98.6,52,-98.6,98.6);
	Float_t wbox	= 12.0*3.8;
	Int_t binx;
	Int_t biny;
	Char_t	sfilename[50];
	sprintf(sfilename,"/home/yuxip/FMS/CellScan/status/status_run%d.txt",runnum);
	fstream statusfile(sfilename,ios::in);
	if(!statusfile){
		cout<<sfilename<<" does not exist!!"<<endl;
		return 0;
	}
	while(statusfile>>ew>>nstb>>ch>>lstatus){
		sscanf(lstatus,"0x%x",&status);
		if((Int_t)status==32||(Int_t)status==64||(Int_t)status==96)status=0;
		cout<<dec<<ew<<" "<<nstb<<" "<<ch<<" "<<"0x"<<(Int_t)status<<endl;
		if(ew==1)continue;
		if(nstb<3){	//large cells
			row0	= (ch-1) / 17;
			col0	= (ch-1) % 17;
			if(!Legal(2,nstb,row0,col0))continue;
			if(!ConvertTH2Bin(nstb,row0,col0,binx,biny,slarge)){
				cout<<"error converting bin numbers for slarge.."<<endl;
				return -1;
			}
			slarge->SetBinContent(binx,biny,(Int_t)status);

		}
		else if(nstb>2){
			row0	= (ch-1) / 12;
			col0	= (ch-1) % 12;
			if(!Legal(2,nstb,row0,col0))continue;
			if(!ConvertTH2Bin(nstb,row0,col0,binx,biny,ssmall)){
                                cout<<"error converting bin numbers for ssmall.."<<endl;
                                return -1;
                        }
                        ssmall->SetBinContent(binx,biny,(Int_t)status);
		}
		else{
			cout<<"wrong nstb number: "<<nstb<<endl;
		}

	}
	
	TLine* lv = new TLine(0,-98.6,0,98.6);
	TLine* lh = new TLine(-98.6,0,98.6,0);
	slarge->SetStats(0);
	slarge->Draw("COLZtext");
	lv->SetLineColor(kRed);
	lv->SetLineWidth(1.5);
	lh->SetLineColor(kRed);
	lh->SetLineWidth(1.5);

	TBox bx(-wbox,-wbox,wbox,wbox);
	bx.SetFillColor(16);
	ssmall->SetStats(0);
	ssmall->Draw("COLZtextsame");
	lv->Draw("same");
	lh->Draw("same");
	bx.Draw("same");

	c1->Print(outfilename);
	return 1;
  }
	
