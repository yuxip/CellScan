
  Int_t AnalyzeCell( Long_t runnum = 12098007,const char* ls = "large" ){
	
	gStyle->SetPalette(1);
	gSystem->Load("../lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	Char_t filename[100];
	sprintf(filename,"/home/yuxip/FMS/CellScan/MyCells_run%ld.root",runnum);
	cout<<"Input file: "<<filename<<endl;
	
	Char_t outfilename[30];
	if(!strcmp(ls,"large")){
		sprintf(outfilename,"LargeCells_run%ld.ps",runnum);
	}
	else if(!strcmp(ls,"small")){
		sprintf(outfilename,"SmallCells_run%ld.ps",runnum);
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
	TFile* infile = new TFile(filename,"read");
	if(!infile){
		cout<<filename<<" does not exist!!"<<endl;
	}
	
	Int_t ncell = 1000;		//for test only
	Int_t cellcnt  = 0;
	Char_t cellname[30];
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
//	Geom*   pGeom 	= new Geom("./","geom.txt");
	TTree* Trpi;
	Float_t	x2, y2;
	Int_t cnt = 1;
	Int_t n1  = 0;
        Int_t n2  = 0;

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
	Int_t largeN1 = 0;
	Int_t smallN1 = 0;
	Int_t largeN2 = 0;
	Int_t smallN2 = 0;

	for(Int_t nstb = n1; nstb <= n2; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){
			for(Int_t col0 = 0; col0 < 17; col0++){

				cout<<"row0: "<<row0<<" col0: "<<col0<<" nstb: "<<nstb<<endl;
				if(!Legal(2,nstb,row0,col0)){
//					cout<<"illegal.."<<endl;
//					cout<<"cnt: "<<cnt<<endl;
					continue;
				}
				cellcnt++;
				sprintf(cellname,"Cellr%d_c%d_n%d",row0,col0,nstb);
				cout<<"processing "<<cellname<<endl;
				mcell = (MyCell*)infile->Get(cellname);
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
	//			c1->GetPad(cnt)->SetLogy();
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
				
				Trpi =  mcell->GetPionSample("all",runnum);
				nentries = Trpi->GetEntries();
				cout<<"nentries: "<<nentries<<endl;

				if(nentries!=0){
					
					cout<<"nentries: "<<nentries<<endl;
					cnt++;
					c1->cd(cnt);
					Trpi->Project("hmass","M","");
					hmass->Draw();
					cnt++;
					c1->cd(cnt);
					if(n1<=2){
						Trpi->Project("hxy1","Y1:X1","(abs(X1)>45.6)||(abs(Y1)>45.6)");
						largeN1 = hxy1->GetBinContent(hxy1->GetMaximumBin());
					}

					if(n1>2){
						Trpi->Project("smallxy1","Y1:X1","(abs(X1)<=45.6)&&(abs(Y1)<=45.6)");
						smallN1 = smallxy1->GetBinContent(smallxy1->GetMaximumBin());
					}

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
					
					hxy1->SetStats(0);
					hxy1->Draw("COLZ");
					TBox bx(-wbox,-wbox,wbox,wbox);
					bx.SetFillColor(18);
					bx.Draw("same");
					smallxy1->SetStats(0);
					smallxy1->Draw("zcolsame");
					cnt++;
					c1->cd(cnt);
					Trpi->Project("hxy2","Y2:X2","(abs(X2)>45.6)||(abs(Y2)>45.6)");
					Trpi->Project("smallxy2","Y2:X2","(abs(X2)<=45.6)&&(abs(Y2)<=45.6)");
					
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

				c1->Print(outfile);
				mcell = 0;
				if(cellcnt == ncell)break;
				c1->Clear();
				hmass->Reset();
				hxy1->Reset();
				hxy2->Reset();
				smallxy1->Reset();
				smallxy2->Reset();
				c1->Divide(2,3);
				cnt = 1;
			}
			if(cellcnt == ncell)break;
		}
		if(cellcnt == ncell)break;
	}
	c1->Print(endout);
  	return 1;
  }
					





