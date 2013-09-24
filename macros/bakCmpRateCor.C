
  Int_t CmpRateCor( Int_t fillnum = 15419 ,const char* sl = "small" ){
	
	gSystem->Load("../lib/libMyCellMgr.so");
        gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	Char_t runlist[20];
	sprintf(runlist,"fill%d.list",fillnum);
	fstream inlist(runlist,ios::in);
	if(!inlist){
		cout<<runlist<<" does not exist!!"<<endl;
		return -1;
	}	

	TH2F* 	fcmp[9];
	TFile* 	fcell	= 0;
	Int_t	n1 	= 0;
	Int_t	n2	= 0;
	
	Char_t outfilename[30];
	if(!strcmp(sl,"large")){
		sprintf(outfilename,"largeRateVsGcor_fill%d_n2.ps",fillnum);
		n1 = 2;
		n2 = 2;         //analysis each nstb separately
	}
	else if(!strcmp(sl,"small")){
		sprintf(outfilename,"smallRateVsGcor_fill%d_n3.ps",fillnum);
		n1 = 3;
		n2 = 3;
	}
	else{
		cout<<"invalid 2nd argument"<<endl;
		return -1;
	}
	TString outfile(outfilename);
	TString headout = outfile + "[";
	TString endout  = outfile + "]";
//	TCanvas* c1 	= new TCanvas("c1","c1",1100,1100);
	TCanvas* c1 	= new TCanvas("c1","c1",700,700);
	//c1->Divide(3,3);
	c1->Print(headout);
	
	fstream inGcor("FmsCorr_sh.txt",ios::in);
	Int_t	iew;
	Int_t	instb;
	Int_t	ich;
	Float_t	gcorsh;
	
	Int_t	cnt	= 1;
	Int_t	cellcnt	= 0;
	Int_t	rch	= 0;
	Long_t 	runnum 	= 0;
	Char_t	cellname[30];
	Char_t	cellfile[30];
	Char_t  title[20];
	Int_t	bit	= 0;
	UChar_t status	= 0;
	Int_t	rate	= 0;
	Int_t 	Trgth	= 100;
	MyCell*	mcell	= 0;
	Float_t mycor;
	Float_t	gdiff;
	while(inlist>>runnum){
	
		sprintf(title,"RateVsGcor_run%ld",runnum);
		fcmp[cnt-1] = new TH2F(title,title,100,-1,1,100,0,10000);
		inGcor.seekg(0,ios::beg);
		while(inGcor>>iew>>instb>>ich>>gcorsh){
			if(instb==n1&&iew==2)break;
		}
	
		for(Int_t nstb = n1; nstb <= n2; nstb++){
			for(Int_t row0 = 0; row0 < 34; row0++){
				for(Int_t col0 = 0; col0 < 17; col0++){

					if(cellcnt)inGcor>>iew>>instb>>ich>>gcorsh;
					cellcnt++;	
					if(!Legal(2,nstb,row0,col0)){
						continue;
					}
					sprintf(cellname,"Cellr%d_c%d_n%d",row0,col0,nstb);
					cout<<"processing "<<cellname<<endl;
					if(col0==0){
						if(nstb<3){
							rch = 17*row0+col0+1;
						}
						else if(nstb<5){
							rch = 12*row0+col0+1;
						}
						cout<<"debug instb: "<<instb<<" nstb: "<<nstb<<" ich: "<<ich<<" rch: "<<rch<<" gcorsh: "<<gcorsh<<endl;
					}

					sprintf(cellfile,"../cells/Cellr%d_c%d_n%d.root",row0,col0,nstb);
					
					fcell = new TFile(cellfile,"read");
					if(!fcell){
						cout<<cellfile<<" does not exist!!"<<endl;
						return -1;
					}
					mcell = (MyCell*)fcell->Get(cellname);
					if(!mcell){
						cout<<"ERROR reading "<<cellname<<" object"<<endl;
						return -1;
					}
					mycor = mcell->GetGainCor();
					if(col0==0)cout<<"mycor: "<<mycor<<endl;
					gdiff = mycor - gcorsh;
//					status = mcell->GetStatusBit(runnum);
//					bit = (Int_t)(status&0x03); //can not be 0011
					rate = mcell->GetNth(runnum,Trgth);
					fcmp[cnt-1]->Fill(gdiff,rate);
					
					mcell	= 0;
					status 	= 0;
					bit	= 0;
					gdiff	= 0;
					fcell->Close();
					fcell	= 0;
				}
			}
		}
		c1->cd(cnt);
		fcmp[cnt-1]->Draw("COLZ");
		if(cnt%9==0){
			c1->Print(outfile);
			c1->Clear();
//			c1->Divide(3,3);
			cnt = 0;
		}
		cnt++;
		cout<<"============================"<<runnum<<" done.."<<endl;
		cellcnt = 0;
	}
	c1->Print(outfile);	
	c1->Print(endout);
		
	return 1;
  }
	


