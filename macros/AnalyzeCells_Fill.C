
  Int_t AnalyzeCells_Fill( const char* runlist = "/home/yuxip/FMS/CellScan/macros/fill15419.list",const char* ls = "small" ){
	
	gStyle->SetPalette(1);
	gSystem->Load("../lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/ConvertTH2Bin.C");
	mGeom = new Geom("./","geom.txt");
	cout<<"runlist: "<<runlist<<endl;
	
	Int_t fillnum = 0;
	Int_t runtoshow = 1;
	Long_t runnum = 0;
	Int_t rcnt = 0;
	sscanf(runlist,"/home/yuxip/FMS/CellScan/macros/fill%d.list",&fillnum);
	fstream infill(runlist,ios::in);
	if(!infill){
		cout<<"ERROR! "<<runlist<<" does not exist!!"<<endl;
		return -1;
	}
	while(infill>>runnum){
		rcnt++;
		if(rcnt==runtoshow)break;
	}
	cout<<"runtoshow: "<<runnum<<endl;
		
	
	Char_t outfilename[30];
	if(!strcmp(ls,"large")){
		sprintf(outfilename,"LargeCells_fill%d_run%ld_n2.ps",fillnum,runnum);
	}
	else if(!strcmp(ls,"small")){
		sprintf(outfilename,"SmallCells_fill%d_run%ld_n4.ps",fillnum,runnum);
	}
	else{
		cout<<"invalid 2nd argument!!"<<endl;
		return -1;
	}
	

	TString outfile(outfilename);
	TString headout = outfile + "[";
	TString endout = outfile + "]";
	
	TCanvas* c1 = new TCanvas("c1","c1",700,1100);
	c1->Divide(2,3);
	c1->Print(headout);
	
	Int_t ncell = 1000;		//for test only
	Int_t cellcnt  = 0;
	Char_t cellname[30];
	Char_t cellfile[30];
	TDirectory* h1where = gDirectory;
	TH1F* hmass 	= new TH1F("hmass","hmass",50,0,1);
	TH2F* hxy1	= new TH2F("hxy1","hxy1",34,-98.6,98.6,34,-98.6,98.6);
	TH2F* hxy2	= new TH2F("hxy2","hxy2",34,-98.6,98.6,34,-98.6,98.6);
	TH2F* smallxy1	= new TH2F("smallxy1","smallxy1",52,-98.6,98.6,52,-98.6,98.6);
	TH2F* smallxy2	= new TH2F("smallxy2","smallxy2",52,-98.6,98.6,52,-98.6,98.6);
	Float_t wbox 	= 12.0*3.8;

	Int_t nentries;
	Int_t 	maxy 	= 0;
	Int_t 	maxx 	= 0;
	UChar_t Status = 0x00;
	Char_t  text[30];
	MyCell* mcell 	= new MyCell();
	TTree* Trpi;
	Float_t	x2, y2;
	Int_t cnt = 1;
	Int_t n1  = 0;
        Int_t n2  = 0;

        if(!strcmp(ls,"large")){
                n1 = 2;
                n2 = 2;		//analysis each nstb separately
        }
        else if(!strcmp(ls,"small")){
                n1 = 4;
                n2 = 4;
        }
        else{
                cout<<"invalid 2nd argument"<<endl;
                return -1;
        }
	Int_t largeN1 = 0;
	Int_t smallN1 = 0;
	Int_t largeN2 = 0;
	Int_t smallN2 = 0;
	TFile* 	fcell = 0;
	MyCellEvt* mcevt = new MyCellEvt();
	
	for(Int_t nstb = n1; nstb <= n2; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){
			for(Int_t col0 = 0; col0 < 17; col0++){

				cout<<"row0: "<<row0<<" col0: "<<col0<<" nstb: "<<nstb<<endl;
				if(!Legal(2,nstb,row0,col0)){
					continue;
				}
				cellcnt++;
				sprintf(cellname,"Cellr%d_c%d_n%d",row0,col0,nstb);
				cout<<"processing "<<cellname<<endl;
				sprintf(cellfile,"../cells/Cellr%d_c%d_n%d.root",row0,col0,nstb);
				
				fcell = new TFile(cellfile,"read");
				mcell = (MyCell*)fcell->Get(cellname);
				mcell->SetGeom(mGeom);
				mcell->Print();
				if(!mcell){
					cout<<"ERROR getting "<<cellname<<endl;
					return 0;
				}
				c1->cd(cnt);
				c1->GetPad(cnt)->SetLogy();
				c1->GetPad(cnt)->SetLogx();
				mcell->GetAdcSpectrum(runnum)->Draw();
				cnt++;
				c1->cd(cnt);
				c1->GetPad(cnt)->SetLogy();
				c1->GetPad(cnt)->SetLogx();
				mcell->GetLEDSpectrum(runnum)->Draw();
				mcell->GetLEDSpectrum(runnum)->SetLineColor(kBlue);
				Status = mcell->GetStatusBit(runnum);
				sprintf(text,"0x%x",Status);
				TLatex l(0.5,0.5,text);
				l.SetNDC();
				l.SetTextColor(2);
				l.Draw();
				cnt++;
				c1->cd(cnt);
//				c1->GetPad(cnt)->SetLogy();
				c1->GetPad(cnt)->SetFillColor(37);
				maxy = mcell->GetLEDvsEvt(runnum)->FindLastBinAbove(0,2);
				maxx = mcell->GetLEDvsEvt(runnum)->FindLastBinAbove(0,1);
				cout<<"maxx: "<<maxx<<endl;
				if(maxy < 2000){
					mcell->GetLEDvsEvt(runnum)->GetYaxis()->SetRange(0,2*maxy);
				}
				if(maxx < 2000){
					mcell->GetLEDvsEvt(runnum)->GetXaxis()->SetRange(0,(maxx+100));
				}
				
				mcell->GetLEDvsEvt(runnum)->Draw("COLZ");
				
				Trpi = mcell->GetPionSample("all",runlist);
				nentries = Trpi->GetEntries();
			//	Trpi->Print();
				cout<<"Trpi nentries: "<<nentries<<endl;
				

				if(nentries!=0){
/*
*/					
//					h1where->cd();
//					cnt++;
//					c1->cd(cnt);
//					Trpi->Project("hmass","M","");
//					hmass->Draw();
//					cnt++;
//					c1->cd(cnt);
					Float_t x1;
                                        Float_t x2;
                                        Float_t y1;
                                        Float_t y2;
					Float_t m;
					Trpi->ResetBranchAddresses();	//the orginal MergedTr was hooked up with transient member MyCellEvt of MyCell
                                        Trpi->SetBranchAddress("mycevt",&mcevt);
//                                        Trpi->SetBranchAddress("Y2",&y2);
//					Trpi->SetBranchAddress("M",&m);
                                        Trpi->SetBranchStatus("*",0);
                                        Trpi->SetBranchStatus("X2",1);
                                        Trpi->SetBranchStatus("Y2",1);
                                        Trpi->SetBranchStatus("M",1);
//					Trpi->GetEntry(0);
//					Trpi->Show(0);
//					cout<<"X2: "<<(mcevt->X2)<<", Y2: "<<(mcevt->Y2)<<", M: "<<(mcevt->M)<<endl;

					Int_t binx;
					Int_t biny;
					TString partner;
					Int_t pnstb = 0;
					Int_t prow0 = 0;
					Int_t pcol0 = 0;
					Long_t prun = 0;
					for(Int_t i = 0; i < nentries; i++){
						
						if(nstb<3){		//large cells
							if(!ConvertTH2Bin(nstb,row0,col0,binx,biny,hxy1)){
								cout<<"ERROR in ConvertTH2Bin()!"<<endl;
								return -1;
							}
							hxy1->SetBinContent(binx,biny,(hxy1->GetBinContent(binx,biny)+1));
						}

						if(nstb>2){		//small cells
							if(!ConvertTH2Bin(nstb,row0,col0,binx,biny,smallxy1)){
                                                                cout<<"ERROR in ConvertTH2Bin()!"<<endl;
                                                                return -1;
                                                        }
							smallxy1->SetBinContent(binx,biny,(smallxy1->GetBinContent(binx,biny)+1));
						}
//						cout<<"binx: "<<binx<<", biny: "<<biny<<endl;
						
						Trpi->GetEntry(i);
//						Trpi->Show(i);
						x2 = mcevt->X2;
						y2 = mcevt->Y2;
						 m = mcevt->M; 
//						cout<<"X2: "<<x2<<", Y2: "<<y2<<", M: "<<m<<endl;
						hmass->Fill(m);
						if(!mGeom->getNSTB(x2,y2))continue;	//bypass abnormal nstb
						partner = mcell->GetPartnerByXY(x2,y2,runnum);
						if(col0==0){
							cout<<"debug: partner: "<<partner<<endl;
						}
						sscanf((const char*)partner,"Cellr%d_c%d_n%d_run%ld",&prow0,&pcol0,&pnstb,&prun);
						if(col0==0){
                                                        cout<<"debug: partner (nstb,row0,col0): ("<<pnstb<<", "<<prow0<<", "<<pcol0<<") "<<endl;
                                                }

						if(!Legal(2,pnstb,prow0,pcol0)){
							cout<<"invalid partner id"<<endl;
							continue;
						}
						if(pnstb<3){             //large cells
							if(!ConvertTH2Bin(pnstb,prow0,pcol0,binx,biny,hxy2)){
                                                                cout<<"ERROR in ConvertTH2Bin()!"<<endl;
                                                                return -1;
                                                        }
							hxy2->SetBinContent(binx,biny,(hxy2->GetBinContent(binx,biny)+1));
						}
						if(pnstb>2){		//small cells
							if(!ConvertTH2Bin(pnstb,prow0,pcol0,binx,biny,smallxy2)){
                                                                cout<<"ERROR in ConvertTH2Bin()!"<<endl;
                                                                return -1;
                                                        }
							smallxy2->SetBinContent(binx,biny,(smallxy2->GetBinContent(binx,biny)+1));
						}
					}
					
					smallN1 = smallxy1->GetBinContent(smallxy1->GetMaximumBin());
					largeN1 = hxy1->GetBinContent(hxy1->GetMaximumBin());
					if(smallN1>=largeN1){
						cout<<"smallN1: "<<smallN1<<endl;
						hxy1->GetZaxis()->SetRangeUser(0,smallN1);
						smallxy1->GetZaxis()->SetRangeUser(0,smallN1);
					}
					else{
						cout<<"largeN1: "<<largeN1<<endl;
						hxy1->GetZaxis()->SetRangeUser(0,largeN1);
                                                smallxy1->GetZaxis()->SetRangeUser(0,largeN1);
					}
	
					cnt++;
                                        c1->cd(cnt);	
					hmass->Draw();

					cnt++;
                                        c1->cd(cnt);
					hxy1->SetStats(0);
					hxy1->Draw("COLZ");
					TBox bx(-wbox,-wbox,wbox,wbox);
					bx.SetFillColor(18);
					bx.Draw("same");
					smallxy1->SetStats(0);
					smallxy1->Draw("zcolsame");
				
					cnt++;
					c1->cd(cnt);
					
					largeN2 = hxy2->GetBinContent(hxy2->GetMaximumBin());
					smallN2 = smallxy2->GetBinContent(smallxy2->GetMaximumBin());
					if(smallN2>=largeN2){
						cout<<"smallN2: "<<smallN2<<endl;
                                                hxy2->GetZaxis()->SetRangeUser(0,smallN2);
                                                smallxy2->GetZaxis()->SetRangeUser(0,smallN2);
                                        }
                                        else{
						cout<<"largeN2: "<<largeN2<<endl;
                                                hxy2->GetZaxis()->SetRangeUser(0,largeN2);
                                                smallxy2->GetZaxis()->SetRangeUser(0,largeN2);
                                        }
					hxy2->SetStats(0);
					hxy2->Draw("COLZ");
					bx.Draw("same");
					smallxy2->SetStats(0);
					smallxy2->Draw("zcolsame");
				}
				largeN1 = largeN2 = 0;
				smallN1 = smallN2 = 0;
				Trpi = 0;
				c1->Print(outfile);
				mcell = 0;
				hmass->Reset();
                                hxy1->Reset();
                                hxy2->Reset();
                                smallxy1->Reset();
                                smallxy2->Reset();
				fcell->Close();
				fcell = 0;
				c1->Clear();
				c1->Divide(2,3);
				cnt = 1;
				if(cellcnt == ncell)break;
				
			}
			if(cellcnt == ncell)break;
		}
		if(cellcnt == ncell)break;
	}
	c1->Print(endout);
  	return 1;
  }
