#include "TObjArray.h"  
#include "TTree.h"
#include "Geom.h"
#include "CalibStr.h"
#include "MyCell.h"
#include "MyCellRunDep.h"
#include "MyCellMgr.h"
#include "MyCellEvt.h"
#include "TEventList.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TSystem.h"
#include "TKey.h"
#include "TGraph.h"
#include "TLine.h"
#include "TLatex.h"

ClassImp(MyCellMgr)

MyCellMgr::MyCellMgr(){

	mGain		= 0;
	mGcor		= 0;
	mGeom		= 0;
//	mInFile		= 0;
	VecMyCell	= 0;
	fout		= 0;
}

//___________________________________________________________________
MyCellMgr::MyCellMgr( Long_t runnum, const char* Outname = "MyCells.root", const char* option = "update" ){

//	mInFile = new ifstream(runlist,ios::in);
//	if(!mInFile){
//		cout<<"ERROR in MyCellMgr constructor -- runlist "<<runlist<<" doesn't exist!!"<<endl;
//		exit(0);
//	}
	mRun = runnum;

	VecMyCell = new TObjArray(1264);
	mOutfile = Outname;
	this->InitFiles();

	if(!strcmp(option,"recreate")){
		cout<<"recreate MyCells Array.."<<endl;
		fout = new TFile(Outname,"recreate");
		if(!InitMyCells()){
		cout<<"In MyCellMgr::MyCellMgr : ERROR initializing MyCells array!!"<<endl;
		exit(0);
		}
	}

	else if(!strcmp(option,"update")||!strcmp(option,"read")){
		cout<<option<<" MyCells Array.."<<endl;
		fout = new TFile(Outname,option);
		Int_t cellcnt = 0;
		if(!fout){
			cout<<"Output file "<<Outname<<" does not exist!!"<<endl;
			exit(0);
		}
		TIter nextkey(fout->GetListOfKeys());
		TKey* key;
		while(key = (TKey*)nextkey()){
			MyCell* mcell = (MyCell*)key->ReadObj();
			mcell->SetGeom(mGeom);
			InsertMyCell(mcell);
			cellcnt++;
		}
		cout<<"cellcnt: "<<cellcnt<<endl;
		if(cellcnt!=1264){
			cout<<"not enough cells!!"<<endl;
			exit(0);
		}	

	}
	
	else{
		cout<<"invalid option"<<endl;
		exit(0);
	}
// 	VecMyCell->Print();
	cout<<"VecMyCell: entries: "<<VecMyCell->GetEntries()<<endl;
	cout<<"MyCellMgr: Output file is "<<Outname<<endl;

}

//___________________________________________________________________
MyCellMgr::~MyCellMgr(){
	
	cout<<"Calling ~MyCellMgr()"<<endl;
	delete mGain;
	delete mGcor;
	delete mGeom;
//	delete mInFile;
	VecMyCell->Delete();
	delete VecMyCell;
	delete fout;
//	mInFile		= 0;
	fout		= 0;
	VecMyCell	= 0;
	mGain		= 0;
	VecMyCell 	= 0;
	mGcor		= 0;
	mGeom		= 0;

}

//___________________________________________________________________
Int_t MyCellMgr::InitFiles(){

	const char* fmsgain = gSystem->Getenv("FMSGAIN");
	if(!fmsgain){
		cout<<"Please set env. $FMSGAIN"<<endl;
		exit(0);
	}

	const char* gaincor = gSystem->Getenv("FMSCORR");
	if(!gaincor){
		cout<<"Please set env. $FMSCORR"<<endl;
		exit(0);
	}

	mGain = new CalibStr(12000000, fmsgain);
	mGcor = new CalibStr(12000000, gaincor);
	mGeom = new Geom("./","geom.txt");

	return 1;

}

//___________________________________________________________________
Int_t MyCellMgr::InitMyCells(){

	Char_t cellname[20];
	MyCell* mycell;	
	for(Int_t nstb = 1; nstb <= 4; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){
			for(Int_t col0 = 0; col0 < 17; col0++){
				
				if(!Legal(2,nstb,row0,col0))continue;
				sprintf(cellname,"Cellr%d_c%d_n%d",row0,col0,nstb);
				mycell = new MyCell(cellname,nstb,row0,col0,mGain,mGcor,mGeom);
				if(!InsertMyCell(mycell)){
					cout<<"ERROR inserting MyCell object "<<cellname<<endl;
					return 0;
				}
			}
		}
		
	}

	return 1;
}

//___________________________________________________________________
Int_t MyCellMgr::InitRunDep(Long_t runnum){
	
	Char_t 		cn[50];
	MyCell* tmpmc 	= 0;
	cout<<"in MyCellMgr::InitRunDep("<<runnum<<") "<<endl;
	for(Int_t nstb = 1; nstb <= 4; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){
			for(Int_t col0 = 0; col0 < 17; col0++){
				if(!Legal(2,nstb,row0,col0))continue;
				sprintf(cn,"Cellr%d_c%d_n%d",row0,col0,nstb);
				if(!(tmpmc=GetCell(cn))){
					cout<<"MyCellMgr::ReadADC() : object "<<cn<<" does not exist!!"<<endl;
					return 0;
				}
				if(!tmpmc->InitMyCellRunDep(runnum)){
					cout<<"MyCellMgr::InitRunDep() : ERROR initializing MyCellRunDep object!!"<<endl;
					return 0;
				}			

			}
		}	
	}
	
	return 1;

}

//___________________________________________________________________
Int_t MyCellMgr::ReadADC( const char* adcdir = ".", Bool_t write = kTRUE ){
	
	Long_t 		runnum = mRun;
	Char_t		fadc[50];
	Char_t		cn[100];
	Int_t		ccnt[4][34][17];
	TFile*		fileadc;
	TTree*		Tr_adc;
	Int_t		nentries;
	Int_t		n1,r0,c0;
	Int_t		led;
	Int_t		adc;
	Int_t		segnum;
	Int_t		evtnum;
	Int_t		aveledrate = 0;
	Long_t 		sumledrate = 0;
	for(Int_t nstb = 0; nstb < 4; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){
			for(Int_t col0 = 0; col0 < 17; col0++){
				ccnt[nstb][row0][col0] = 0;
			}
		}
	}

//	while(*(mInFile)>>runnum){
		
		cout<<"MyCellMgr: ReadADC() for run: "<<runnum<<endl;
		sprintf(fadc,"%s/adcTr_run%ld.root",adcdir,runnum);
		if(!this->InitRunDep(runnum)){						//initialize MyCellRunDep for all cells		
			cout<<"MyCellMgr::ReadADC() : ERROR initializing MyCellRunDep.."<<endl;
			return 0;
		}
//		mDir = gDirectory;
		
		cout<<"Adc file: "<<fadc<<endl;
		fileadc = new TFile(fadc,"read");
		if(!fileadc){
			cout<<"ERROR in MyCellMgr::ReadADC() : file "<<fadc<<"does not exist!!"<<endl;
			return 0;
		}
		Tr_adc = (TTree*)fileadc->Get("Tr_adc");
		Tr_adc->SetBranchAddress("br_ADC",&adc);
		Tr_adc->SetBranchAddress("br_nstbADC",&n1);
		Tr_adc->SetBranchAddress("br_rowADC",&r0);
		Tr_adc->SetBranchAddress("br_colADC",&c0);
		Tr_adc->SetBranchAddress("br_led",&led);
		Tr_adc->SetBranchAddress("br_segnum",&segnum);
		Tr_adc->SetBranchAddress("br_evtnum",&evtnum);
		
		Tr_adc->SetBranchStatus("*",0);
		Tr_adc->SetBranchStatus("br_ADC",1);
		Tr_adc->SetBranchStatus("br_nstbADC",1);
		Tr_adc->SetBranchStatus("br_rowADC",1);
		Tr_adc->SetBranchStatus("br_colADC",1);
		Tr_adc->SetBranchStatus("br_led",1);
		Tr_adc->SetBranchStatus("br_segnum",1);
		Tr_adc->SetBranchStatus("br_evtnum",1);
		
		
		nentries = Tr_adc->GetEntries();
		cout<<"MyCellMgr::ReadADC() : # of events:"<<nentries<<endl;
//		nentries = 200000;	//for test
		for(Int_t cnt = 0; cnt < nentries; cnt++){
				
				if(cnt%100000==0)cout<<"event: "<<cnt<<endl;
				Tr_adc->GetEntry(cnt);
				sprintf(cn,"Cellr%d_c%d_n%d",r0,c0,n1);

				if(cnt==0){
					cout<<"debug-------MyCellMgr::ReadADC(): processing "<<cn<<endl;
					cout<<"ADC:	"<<adc<<endl;
					cout<<"led:	"<<led<<endl;
					cout<<"segnum:	"<<segnum<<endl;
					cout<<"evtnum:	"<<evtnum<<endl;
					cout<<endl;
				}

				if(led==0){
					this->GetCell(cn)->GetAdcSpectrum(runnum)->Fill(adc);
				}
				else{
					this->GetCell(cn)->GetLEDSpectrum(runnum)->Fill(adc);
				//	gevt = (segnum-1)*10000+evtnum;
					this->GetCell(cn)->GetLEDvsEvt(runnum)->Fill(ccnt[n1-1][r0][c0],adc);
					ccnt[n1-1][r0][c0]++;	//sequence #, instead of event #
					sumledrate++;
				}

		}

	aveledrate = (Int_t)(sumledrate / 1264); 
	cout<<"sumledrate: "<<sumledrate<<endl;
	cout<<"aveledrate: "<<aveledrate<<endl;
	cout<<"setting cell status.."<<endl;
	this->SetCellStatus(0.5,aveledrate,write);
//	}//while
//	mInFile->clear();
//	mInFile->seekg(0,ios::beg);


//Write MyCell object to file
//	cout<<"==================================================================="<<endl;
//	gDirectory->ls();
//	VecMyCell->Print();
	cout<<"VecMyCell: nentries: "<<VecMyCell->GetEntries()<<endl;
	if(write){
		this->WriteCellArray();
	}
	fileadc->Close();
//	cout<<"==================================================================="<<endl;
//	gDirectory->ls();
		
	return 1;
}

//___________________________________________________________________
Int_t MyCellMgr::ReadOutput( const char* ofdir = "." ,TCut cuts = "all", Bool_t write = kTRUE){		
										//populate the Pion sample for each MyCellRunDep object
	
	Long_t          runnum = mRun;
        MyCellRunDep*   mcrd = 0;
        Char_t          cn[100];
        MyCell*         tmpmc;
        Char_t          mcrdname[100];
        Char_t          fname[100];
	Int_t		nentries;
	TFile*		foutput;
	TTree*		TwoTr = 0;
	MyCellEvt	mcevt;							//temporary containter
	Int_t           n1,r0,c0;
	Int_t		np1,rp0,cp0;
	TVector3	lvec,gvec;
	
//	while(*(mInFile)>>runnum){
		cout<<"MyCellMgr::ReadOutput() for run: "<<runnum<<endl;
		sprintf(fname,"%s/OutputNEWBBCAsy_run%ld.root",ofdir,runnum);
		cout<<"Input file: "<<fname<<endl;
		if(!this->InitRunDep(runnum)){					//initialize MyCellRunDep for all cells         
			cout<<"MyCellMgr::ReadOutput() : ERROR initializing MyCellRunDep.."<<endl;
			return 0;
		}
		foutput = new TFile(fname,"read");
		if(!foutput){
			cout<<"fname doesn't exist!!"<<endl;
			return 0;
		}

		TwoTr = (TTree*)foutput->Get("TwoTr");
		TwoTr->SetBranchAddress("M12",&(mcevt.M));
		TwoTr->SetBranchAddress("E12",&(mcevt.E));
		TwoTr->SetBranchAddress("C12",&(mcevt.C12));
		TwoTr->SetBranchAddress("Y1",&(mcevt.Eta1));
		TwoTr->SetBranchAddress("Y2",&(mcevt.Eta2));
		TwoTr->SetBranchAddress("Phi1",&(mcevt.Phi1));
		TwoTr->SetBranchAddress("Phi2",&(mcevt.Phi2));
		TwoTr->SetBranchAddress("Z",&(mcevt.Z));
		TwoTr->SetBranchAddress("Rnum",&(mcevt.Rnum));
		TwoTr->SetBranchAddress("Eta",&(mcevt.Eta));
		TwoTr->SetBranchAddress("Phi",&(mcevt.Phi));
//		TwoTr->SetBranchAddress("X",&(mcevt.X));	//TwoTr structure was changed for OutputPSU_run*.root
//		TwoTr->SetBranchAddress("Y",&(mcevt.Y));
//		TwoTr->SetBranchAddress("ievt",&(mcevt.Evt));
//		TwoTr->SetBranchAddress("TrigBits",&(mcevt.TrigBits));//JP1 for now, 02/07/2012
//		TwoTr->SetBranchAddress("pClusters",&(mcevt.pClusters));//Yuxi 02/07/2012

		TwoTr->SetBranchStatus("*",0);
		TwoTr->SetBranchStatus("N12",1);
		TwoTr->SetBranchStatus("M12",1);
		TwoTr->SetBranchStatus("E12",1);
		TwoTr->SetBranchStatus("C12",1);
		TwoTr->SetBranchStatus("Y1",1);
		TwoTr->SetBranchStatus("Y2",1);
		TwoTr->SetBranchStatus("Phi1",1);
		TwoTr->SetBranchStatus("Phi2",1);
		TwoTr->SetBranchStatus("Z",1);
		TwoTr->SetBranchStatus("Rnum",1);
		TwoTr->SetBranchStatus("Eta",1);
		TwoTr->SetBranchStatus("Phi",1);
//		TwoTr->SetBranchStatus("X",1);
//		TwoTr->SetBranchStatus("Y",1);
		TwoTr->SetBranchStatus("Ntracks",1);
		TwoTr->SetBranchStatus("Esoft",1);
		TwoTr->SetBranchStatus("TrigBits",1);
//		TwoTr->SetBranchStatus("ievt",1);
//		TwoTr->SetBranchStatus("pClusters",1);
		
		TEventList *elist = new TEventList("elist","elist",10000);
		nentries = TwoTr->GetEntries();
		
		if(strcmp(cuts,"all")){							//cuts to select pion sample
			cout<<"cut applied.."<<endl;
			cout<<"cut: "<<cuts<<endl;
			TwoTr->Draw(">>elist",cuts);
			nentries = elist->GetN();
		}
			
		cout<<"MyCellMgr::ReadOutput() : # of events:"<<nentries<<endl;
		for(Int_t i = 0; i < nentries; i++){
			if(i%100000==0)cout<<"ReadOutput() event: "<<i<<endl;
			if(strcmp(cuts,"all")){							//cuts to select pion sample
				cout<<"getting events from elist"<<endl;
				TwoTr->GetEvent(elist->GetEntry(i));
			}
			else{
				TwoTr->GetEntry(i);
			}
			
			cout<<"after getting events.."<<endl;
			mcevt.E1 = ((1 + mcevt.Z)*mcevt.E) / 2;
			mcevt.E2 = ((1 - mcevt.Z)*mcevt.E) / 2;
			this->GetPointXY((mcevt.Eta1),(mcevt.Phi1),(mcevt.X1),(mcevt.Y1),(n1));	//high energy daughter photon
			this->GetPointXY((mcevt.Eta2),(mcevt.Phi2),(mcevt.X2),(mcevt.Y2),(np1));
			if(n1==0||np1==0)continue;
			if(i==0){
				cout<<"debug-------MyCellMgr::ReadOutput() "<<endl;
				cout<<"M12:     "<<(mcevt.M)<<endl;
				cout<<"runnum:  "<<(mcevt.Rnum)<<endl;
//				cout<<"evtnum:  "<<(mcevt.Evt)<<endl;
				cout<<"E1:	"<<(mcevt.E1)<<endl;
				cout<<"E2:	"<<(mcevt.E2)<<endl;
				cout<<"Eta1:	"<<(mcevt.Eta1)<<endl;
				cout<<"Phi1:	"<<(mcevt.Phi1)<<endl;
				cout<<"Eta2:	"<<(mcevt.Eta2)<<endl;
				cout<<"Phi2:	"<<(mcevt.Phi2)<<endl;
				cout<<"X1:	"<<(mcevt.X1)<<endl;
				cout<<"Y1:	"<<(mcevt.Y1)<<endl;
				cout<<"nstb1:	"<<n1<<endl;
				cout<<"X2:	"<<(mcevt.X2)<<endl;
				cout<<"Y2:	"<<(mcevt.Y2)<<endl;
				cout<<"nstb2:	"<<np1<<endl;
			}
			gvec.SetXYZ(mcevt.X1,mcevt.Y1,0);
			lvec 	= mGeom->LocalXYZ(2,n1,gvec,true);
			r0	= lvec.Y();
			c0	= lvec.X();						
			sprintf(cn,"Cellr%d_c%d_n%d",r0,c0,n1);
			if(i==0)cout<<"Primary cell: "<<cn<<endl;
			if(!this->GetCell(cn))continue;				//for test
			mcrd = (MyCellRunDep*)(this->GetCell(cn)->GetMyCellrd(runnum));
			mcrd->Print();
			if(!mcrd){
				cout<<"MyCellMgr::ReadOutput() : ERROR getting MyCellRunDep object!!"<<endl;
				exit(0);
			}
			
			mcrd->SetMyCellEvt(&mcevt);
			if(i==0){
                                cout<<"mcrd debug-------MyCellMgr::ReadOutput() "<<endl;
                                cout<<"M12:     "<<(mcrd->GetMyCellEvt()->M)<<endl;
                                cout<<"runnum:  "<<(mcrd->GetMyCellEvt()->Rnum)<<endl;
				cout<<"energy:  "<<(mcrd->GetMyCellEvt()->E)<<endl;
/*				cout<<"E1:      "<<(mcevt.E1)<<endl;
				cout<<"E2:      "<<(mcevt.E2)<<endl;
				cout<<"Eta1:    "<<(mcevt.Eta1)<<endl;
				cout<<"Phi1:    "<<(mcevt.Phi1)<<endl;
				cout<<"Eta2:    "<<(mcevt.Eta2)<<endl;
				cout<<"Phi2:    "<<(mcevt.Phi2)<<endl;
				cout<<"X1:      "<<(mcevt.X1)<<endl;
				cout<<"Y1:      "<<(mcevt.Y1)<<endl;
                                cout<<"nstb1:   "<<n1<<endl;
                                cout<<"X2:      "<<(mcevt.X2)<<endl;
                                cout<<"Y2:      "<<(mcevt.Y2)<<endl;
                                cout<<"nstb2:   "<<np1<<endl;*/
			}
//			gDebug = 2;
			mcrd->GetPionSample("all")->Fill();
//			mcrd->GetPionSample("all")->Show(0);
//			mcrd->GetPionSample("all")->Print();
			mcrd->GetMyCellEvt()->Clear();

		}//for
	
	elist->Reset();
	delete elist;
	elist = 0;


//	}//while
	cout<<"finished while.."<<endl;
//	delete mcrd;
	mcrd = 0;
	cout<<"MyCellMgr::ReadOutput(), VecMyCell: nentries: "<<VecMyCell->GetEntries()<<endl;
	if(write){
		this->WriteCellArray();
	}
//	foutput->Close();
//	mInFile->clear();
//	mInFile->seekg(0,ios::beg);


	return 1;

}

//___________________________________________________________________
Int_t MyCellMgr::SetCellStatus(Float_t LedBadTh = 0.5, Int_t AveLEDRate = 1000, Bool_t write = kFALSE){

//called after ReadADC()
//1. average trigger rate. checked with gaincor(==1?, no calibration info),label cold cells
//2. average LED rms / mean --> noisy cells
//3. encode bitshift

	Int_t 	AdcTh 	= 100;
	Int_t	nCellsOverThLrg = 0;
	Int_t	nCellsOverThSml = 0;
	Int_t	TrigRate[4][34][17];
	for(Int_t nstb = 0; nstb < 4; nstb++){
                for(Int_t row0 = 0; row0 < 34; row0++){
                        for(Int_t col0 = 0; col0 < 17; col0++){
				TrigRate[nstb][row0][col0] = 0;
			}
		}
	}

	Int_t 	LedRateTh;		//nominal LED events, if less ->broken led, or cold cell
	Int_t 	BitShift;
	Float_t PreGainCor;		//if == 1, no valid calibration
	Float_t	LedRms;
	Float_t LedMean;
	Int_t	LedRate;
//	Float_t	LedBadTh = 0.5;		//if LedRms / LedMean >= LedBadTh : unstable cell
	
	UChar_t Status	 = 0x00;	//8 bits
	//===================== Status bit ==========================//
	//bit 0: set to 1 if TrigRate < AveRate - 3*RateSpread -->modified to < AveRate / 10
	//bit 1: set to 1 if TrigRate > AveRate + 3*RateSpread
	//bit 2: set to 1 if GainCor == 1. No valid calibration
	//bit 3: set to 1 if LED rate < 0.5* nominal # of LED event
	//bit 4: set to 1 if LedRms / LedMean (relative gain variation) >= LedBadTh. Unstable cell
	//bit 5, 6, 7 : encoded bit shift, from 0 to 7
	//===========================================================//
	
	Char_t	cellname[30];
	MyCell* mcell = 0;


	Float_t AveRate[2];		//AveRate[0]: large cells;  AveRate[1]: small cells
	Float_t RateSpread[2]; 
	Int_t	tmpbin;
	this->GetTrigRate( mRun, AdcTh, AveRate, RateSpread, TrigRate, write );	//produce # over threshold vs ch# plot
	cout<<"AveRate[0]: 	"<<AveRate[0]<<endl;
	cout<<"RateSpread[0]: 	"<<RateSpread[0]<<endl;
	cout<<"AveRate[1]: 	"<<AveRate[1]<<endl;
	cout<<"RateSpread[1]: 	"<<RateSpread[1]<<endl;
	for(Int_t nstb = 1; nstb < 5; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){
			for(Int_t col0 = 0; col0 < 17; col0++){
				
				if(!Legal(2,nstb,row0,col0))continue;
				Status = 0x00;
				sprintf(cellname,"Cellr%d_c%d_n%d",row0,col0,nstb);
				cout<<"setting status for "<<cellname<<endl;
				if(!(this->GetCell(cellname))){
					cout<<"In MyCellMgr::SetCellStatus() : ERROR getting object "<<cellname<<endl;
					exit(0);
				}
				mcell = (MyCell*)(this->GetCell(cellname));	
				if(col0==0){
					cout<<"debug-- TrigRate: "<<TrigRate[nstb-1][row0][col0]<<endl;
				}
//				cout<<"before anything, Status: "<<hex<<(Int_t)Status<<endl;
				if(TrigRate[nstb-1][row0][col0]>0){
					if(nstb<3)nCellsOverThLrg++;
					if(nstb>2&&nstb<5)nCellsOverThSml++;
				}
//				if(TrigRate[nstb-1][row0][col0] < (AveRate[(Int_t)((nstb-0.2)/2)] - 3*RateSpread[(Int_t)((nstb-0.2)/2)])){
				if(TrigRate[nstb-1][row0][col0] < (AveRate[(Int_t)((nstb-0.2)/2)])/10){
//					cout<<"bit 0: 1"<<endl;
					Status+=0x01;
				}
//				cout<<"after bit 0, Status: 0x"<<hex<<(Int_t)Status<<endl;
				if(TrigRate[nstb-1][row0][col0] > (AveRate[(Int_t)((nstb-0.2)/2)] + 3*RateSpread[(Int_t)((nstb-0.2)/2)])){
//					cout<<"bit 1: 1"<<endl;
					Status+=0x02;
				}
//				cout<<"after bit 1, Status: 0x"<<hex<<(Int_t)Status<<endl;
				if(mcell->GetGainCor() == 1){
//					cout<<"bit 2: 1"<<endl;
					Status+=0x04;
				}
//				cout<<"after bit 2, Status: 0x"<<hex<<(Int_t)Status<<endl;

				LedRms 	= mcell->GetLEDSpectrum(mRun)->GetRMS();
				LedMean	= mcell->GetLEDSpectrum(mRun)->GetMean();
				LedRate = mcell->GetLEDSpectrum(mRun)->GetEntries();
				if(LedRate < 0.5*AveLEDRate){
//					cout<<"bit 3: 1"<<endl;
					Status+=((0x01)<<3);
				}
//				cout<<"after bit 3, Status: 0x"<<hex<<(Int_t)Status<<endl;
					
				cout<<dec<<endl;
				if((LedRms / LedMean) > LedBadTh)Status+=((0x01)<<4);
				tmpbin 	= mcell->GetAdcSpectrum(mRun)->FindFirstBinAbove(0,1);
				BitShift = (Int_t)(log(mcell->GetAdcSpectrum(mRun)->GetBinLowEdge(tmpbin))/log(2));
				if(BitShift<0||BitShift>7){
					cout<<"BitShift too big: "<<BitShift<<endl;
					BitShift = 0;
				}
	
				Status+=((BitShift)<<5);
			
				if(col0==0){
					cout<<"LedRms: 		"<<LedRms<<endl;
					cout<<"LedMean: 	"<<LedMean<<endl;
					cout<<"LedRate:		"<<LedRate<<endl;
				//	cout<<"binlowedge: 	"<<mcell->GetAdcSpectrum(mRun)->GetBinLowEdge(tmpbin)<<endl;
					cout<<"BitShift:	"<<BitShift<<endl;
					cout<<"Status:		0x"<<hex<<(Int_t)Status<<endl;
					cout<<dec<<endl;
				}
				mcell->SetStatusBit(mRun,Status);
			}
		}
	}
	cout<<"nCellsOverThLrg: "<<nCellsOverThLrg<<endl;					
	cout<<"nCellsOverThSml: "<<nCellsOverThSml<<endl;					
	return 1;

}

//___________________________________________________________________
Int_t MyCellMgr::GetPointXY(Float_t eta, Float_t phi, Float_t& x, Float_t& y, Int_t& nstb){

	Float_t theta 	= 2*atan(TMath::Exp(-eta));
	Float_t zz	= *(mGeom->ZFPD(2,3));			//take small cell zoffset temporarily	
	Float_t pt	= zz*tan(theta);
	TVector3 vec;
	vec.SetPtEtaPhi(pt,eta,phi);
	x 		= vec.X();
	y 		= vec.Y();

	nstb = mGeom->getNSTB(x,y);
	if(nstb==0){
		cout<<"ERROR MyCellMgr::GetPointXY()"<<endl;
//		exit(0);
	}
	if(nstb<3){					//large cells
		
		zz	= *(mGeom->ZFPD(2,1));
		pt	= zz*tan(theta);
		vec.SetPtEtaPhi(pt,eta,phi);
		x 	= vec.X();
		y 	= vec.Y();
		if(nstb==0){
			nstb = mGeom->getNSTB(x,y);
			cout<<"corrected nstb: "<<nstb<<endl;
		}
	}

	return 1;
}

//___________________________________________________________________
MyCellRunDep* MyCellMgr::GetPartnerByXY( Float_t x2, Float_t y2, Long_t runnum ){
	
	//under construction...
	return 0;

}

//___________________________________________________________________
Int_t MyCellMgr::GetTrigRate( Long_t runnum, Int_t threshold = 10, Float_t* AveRate = 0, Float_t* RateSpread = 0, Int_t (*TrigRate)[34][17] = 0 ,Bool_t write = kFALSE ){


	Char_t cellname[30];
	MyCell* mcell = 0;
	Long_t	sumrate[2];
	TGraph* LrgRate = new TGraph(0);
	Char_t 	Lrgname[30];
	sprintf(Lrgname,"LrgRates_run%ld",runnum);
	TGraph* SmlRate = new TGraph(0);
	Char_t 	Smlname[30];
	sprintf(Smlname,"SmlRates_run%ld",runnum);
	Int_t	channellrg = 0;
	Int_t	channelsml = 0;
	Int_t	rate = 0;
	sumrate[0] = sumrate[1] = 0;
	for(Int_t nstb = 1; nstb < 5; nstb++){
		for(Int_t row0 = 0; row0 < 34; row0++){
			for(Int_t col0 = 0; col0 < 17; col0++){

				if(!Legal(2,nstb,row0,col0))continue;
				sprintf(cellname,"Cellr%d_c%d_n%d",row0,col0,nstb);
				if(!(this->GetCell(cellname))){
                                        cout<<"In MyCellMgr::SetCellStatus() : ERROR getting object "<<cellname<<endl;
                                        exit(0);
                                }
                                mcell = (MyCell*)(this->GetCell(cellname));
				rate  = mcell->GetNth(runnum,threshold);
				TrigRate[nstb-1][row0][col0] = rate;
				if(nstb<3){
					sumrate[0] += rate;
					LrgRate->SetPoint(channellrg,channellrg,rate);
					channellrg++;
				}
				else if(nstb>2&&nstb<5){
					sumrate[1] += rate;
					SmlRate->SetPoint(channelsml,channelsml,rate);
					channelsml++;
				}
			}
		}
	}

	AveRate[0] = (Float_t)(sumrate[0]/788);
	AveRate[1] = (Float_t)(sumrate[1]/476);
	
	Float_t sumdiff[2];
	Int_t cntlarge = 0;
	Int_t cntsmall = 0;
	sumdiff[0] = sumdiff[1] = 0;
	for(Int_t nstb = 1; nstb < 5; nstb++){
                for(Int_t row0 = 0; row0 < 34; row0++){
                        for(Int_t col0 = 0; col0 < 17; col0++){

                                if(!Legal(2,nstb,row0,col0))continue;
				if(nstb<3){
					sumdiff[0] += TMath::Power((TrigRate[nstb-1][row0][col0]-AveRate[0]),2);
					cntlarge++;
				}
				else if(nstb>2&&nstb<5){
					sumdiff[1] += TMath::Power((TrigRate[nstb-1][row0][col0]-AveRate[1]),2);
					cntsmall++;
				}	
			}
		}
	}
	RateSpread[0] = TMath::Sqrt(sumdiff[0]/788);	
	RateSpread[1] = TMath::Sqrt(sumdiff[1]/476);	

	Float_t lrghigh = AveRate[0] + 3*RateSpread[0];
	Float_t lrglow 	= AveRate[0] / 10;
	Float_t smlhigh = AveRate[1] + 3*RateSpread[1];
	Float_t smllow 	= AveRate[1] / 10;


	TCanvas* c1 = new TCanvas("c1","c1",800,700);
	c1->SetLogy();
	LrgRate->SetMarkerStyle(2);
	LrgRate->SetMarkerColor(kRed);
	LrgRate->GetXaxis()->SetRangeUser(0,800);
	LrgRate->SetTitle(Lrgname);
	LrgRate->Draw("Ap");

	TLine lrgmean(0,AveRate[0],800,AveRate[0]);
	lrgmean.SetLineColor(kRed);
	lrgmean.Draw();

	TLine llhigh(0,lrghigh,800,lrghigh);
	llhigh.SetLineStyle(7);
	llhigh.Draw();
	
	TLine llow(0,lrglow,800,lrglow);
	llow.SetLineStyle(7);
	llow.Draw();

	Char_t largegif[20];
	sprintf(largegif,"%s.gif",Lrgname);
	Char_t lrgtext1[30];
	Char_t lrgtext2[30];
	sprintf(lrgtext1,"AveRate: %.1f",AveRate[0]);
	sprintf(lrgtext2,"RateSpread: %.1f",RateSpread[0]);
	TLatex l1(0.6,0.8,lrgtext1);
	l1.SetNDC();
	l1.SetTextSize(0.05);
	l1.Draw();
	TLatex l2(0.6,0.75,lrgtext2);
	l2.SetTextSize(0.05);
	l2.SetNDC();
	l2.Draw();

	c1->Print(largegif);
	c1->Clear();

	Char_t smallgif[20];
	sprintf(smallgif,"%s.gif",Smlname);
	SmlRate->SetMarkerStyle(2);
	SmlRate->SetMarkerColor(kBlue);
	SmlRate->GetXaxis()->SetRangeUser(0,500);
	SmlRate->SetTitle(Smlname);
	SmlRate->Draw("Ap");

	TLine smlmean(0,AveRate[1],500,AveRate[1]);
	smlmean.SetLineColor(kBlue);
	smlmean.Draw();

	TLine lshigh(0,smlhigh,500,smlhigh);
	lshigh.SetLineStyle(7);
	lshigh.Draw();
	
	TLine lsow(0,smllow,500,smllow);
	lsow.SetLineStyle(7);
	lsow.Draw();

	Char_t smltext1[30];
	Char_t smltext2[30];
	sprintf(smltext1,"AveRate: %.1f",AveRate[1]);
	sprintf(smltext2,"RateSpread: %.1f",RateSpread[1]);
	TLatex ls1(0.6,0.8,smltext1);
	ls1.SetNDC();
	ls1.SetTextSize(0.05);
	ls1.Draw();
	TLatex ls2(0.6,0.75,smltext2);
	ls2.SetTextSize(0.05);
	ls2.SetNDC();
	ls2.Draw();
	
	c1->Print(smallgif);
	fout->cd();
	if(write){
		LrgRate->Write(Lrgname,TObject::kOverwrite);
		SmlRate->Write(Smlname,TObject::kOverwrite);
	}
	
	return 1;

}

//___________________________________________________________________
Int_t MyCellMgr::InsertMyCell( MyCell* mycell ){
	
	VecMyCell->Add(mycell);
	return 1;

}

//___________________________________________________________________
MyCell* MyCellMgr::GetCell( const char* cname ){
	
	Int_t n1,r0,c0;
	
	if(strncmp(cname,"Cellr",5)){
                cout<<"MyCellMgr::GetCell() : wrong name format: "<<endl;
                cout<<"Required format Cellr*_d*_n*"<<endl;
                return 0;
        }
	sscanf(cname,"Cellr%d_c%d_n%d",&r0,&c0,&n1);
	if(!Legal(2,n1,r0,c0)){
		cout<<"MyCellMgr::GetCell() : ("<<n1<<", "<<r0<<", "<<c0<<"), "<<"illegal (nstb,row,col) triplet!!"<<endl;
		return 0;
	}

	MyCell* mycell = (MyCell*)VecMyCell->FindObject(cname);
	if(!mycell){
		cout<<"MyCellMgr::GetCell() : didn't find object "<<cname<<endl; //commented out for test
		return 0;
	}
	
	return mycell;

}

//___________________________________________________________________
Int_t MyCellMgr::WriteCellArray(){
	
	fout->cd();
	VecMyCell->Write("",TObject::kOverwrite);
	return 1;

}

//___________________________________________________________________
Bool_t MyCellMgr::Legal( Int_t iew, Int_t nstb, Int_t row0, Int_t col0 ){

	if(iew>0 && iew<2)				return false;
	if(nstb<1 || nstb>4)				return false;
	if(nstb>2){
		if(row0<0 || row0>23)			return false;
		if(col0<0 || col0>11)			return false;
		if(fabs(1.*row0-11.5)<5 && col0<5)	return false;
	}
	else{
		if(row0<0 || row0>33)			return false;
		if(col0<0 || col0>16)			return false;
		if(fabs(1.*row0-16.5)<8 && col0<8)	return false;
		if(row0<col0-9.5)			return false;
		if(33-row0<col0-9.5)			return false;
	}

	return true;
}		
