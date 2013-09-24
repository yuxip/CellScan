//correlations between count rate( # over threshold ) vs LED corrected gcorr, to check outliers
//--Yuxi Pan 04/03/2012
  Int_t RateVsEta(Long_t run = 12098007){

	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	const char* celldir = "/home/yuxip/FMS/CellScan/cells/psu";

	TGraph*	greta[2];	//small/large
	Char_t	gname[50];
	TCanvas* c1 = new TCanvas("c1","c1",800,400);
	c1->Divide(2,1);
	Char_t 	outfilename[40];
	sprintf(outfilename,"gRateVsEta_run%ld.ps",run);
	TString outfile(outfilename);

	TFile*	fdata;
	Char_t	fdatafile[80];
	Char_t	cellname[50];
	MyCell* fcell;
	Int_t	AdcTh = 100;	//adc threshold
	Int_t	icell[2] = {0,0};

	for(Int_t sl = 0; sl < 2; sl++){
		if(sl==0){
			sprintf(gname,"RateVsEta_run%ld_LargeCells",run);
			greta[sl] = new TGraph(0);
			greta[sl]->SetTitle(gname);
		}
		else {
			sprintf(gname,"RateVsEta_run%ld_SmallCells",run);
                        greta[1] = new TGraph(0);
                        greta[1]->SetTitle(gname);
		}
	}
	
	for(Int_t nstb = 1; nstb < 5; nstb++){
					
		Int_t ls = nstb/3;
		Int_t nrow,ncol;
		if(nstb<3){
			nrow = 34;
			ncol = 17;
		}
		else{
			nrow = 24;
			ncol = 12;
		}
		for(Int_t row = 0; row < nrow; row++){
			for(Int_t col = 0; col < ncol; col++){
				if(!Legal(2,nstb,row,col))continue;
					
					sprintf(cellname,"Cellr%d_c%d_n%d",row,col,nstb);
					sprintf(fdatafile,"%s/%s.root",celldir,cellname);
					fdata = new TFile(fdatafile,"read");
					if(!fdata){
						cout<<fdatafile<<" does not exist!!"<<endl;
						return 0;
					}
					fcell = (MyCell*)fdata->Get(cellname);
					if(!fcell){
						cout<<"ERROR!! "<<cellname<<" object does not exist!!"<<endl;
						return 0;
					}	
					
					Float_t Ntriggers = fcell->GetNth(run,AdcTh);
					Float_t Eta	  = fcell->GetEta();
					if(Ntriggers>8000){
						cout<<"hot cell: "<<cellname<<endl;
						cout<<"Ntriggers	= "<<Ntriggers<<endl;
						cout<<"Eta		= "<<Eta<<endl;
					}
					//if(Eta<1||Ntriggers==0)continue;
					/*if(icell[ls]%50==0){
						cout<<"debug "<<cellname<<endl;
						cout<<"Ntriggers	= "<<Ntriggers<<endl;
						cout<<"Eta		= "<<Eta<<endl;
					}*/
					greta[ls]->SetPoint(icell[ls],Eta,Ntriggers);
					icell[ls]++;
					fdata->Close();
					delete fdata;
					fdata = 0;
				}
			}
		}
		cout<<"# large cells: "<<icell[0]<<endl;
		cout<<"# small cells: "<<icell[1]<<endl;
		for(Int_t i = 0; i < 2; i++){
			c1->cd(i+1);
			cout<<"greta["<<i<<"] Npoints: "<<greta[i]->GetN()<<endl;
			greta[i]->SetMarkerStyle(2);
			greta[i]->SetMarkerColor(i+1);
			greta[i]->Draw("Ap");
		}
	c1->Print(outfile);	
	return 1;
  }				
