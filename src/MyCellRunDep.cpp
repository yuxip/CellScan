#include "TClonesArray.h"
#include "TTree.h"
#include "Geom.h"
#include "CalibStr.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCut.h"
#include "TEventList.h"

#include "MyCellRunDep.h"
#include "MyCellEvt.h"

ClassImp(MyCellRunDep)

MyCellRunDep::MyCellRunDep():TObject(){
	
	mAdc 		= 0;
	mLed 		= 0;
	mLedEvt 	= 0;
	mMyCellTr	= 0;
	mMyCevt		= 0;

}

//___________________________________________________________________
MyCellRunDep::MyCellRunDep( const char* name ){
	
//	cout<<"Initialzing MyCellRunDep with name "<<name<<endl;
	
	mMyCellTr 	= 0;
	mStatus 	= 0;
	mAdc		= 0;
	mLed		= 0;
	mLedEvt		= 0;
	mMyCevt		= 0;

	if(!SetName(name)){
		cout<<"ERROR setting name for MyCellRunDep"<<endl;
		exit(0);
	}
	
//	cout<<"this->GetName(): "<<this->GetName()<<endl;
	sscanf(this->GetName(),"Cellr%d_c%d_n%d_run%ld",&mRow,&mCol,&mNstb,&mRun);
//	cout<<"Row: "<<mRow<<" Col: "<<mCol<<" Nstb: "<<mNstb<<endl;
//	cout<<"Run number: "<<mRun<<endl;

	Char_t adcname[100];
	sprintf(adcname,"hadc_r%d_c%d_n%d_run%ld",mRow,mCol,mNstb,mRun);
	mAdc = new TH1F(adcname,adcname,4096,0,4096);
  
	Char_t ledname[100];
	sprintf(ledname,"hled_r%d_c%d_n%d_run%ld",mRow,mCol,mNstb,mRun);
	mLed = new TH1F(ledname,ledname,4096,0,4096);
	
	Char_t ledevtname[100];
	sprintf(ledevtname,"hledevt_r%d_c%d_n%d_run%ld",mRow,mCol,mNstb,mRun);
	mLedEvt = new TH2F(ledevtname,ledevtname,500,0,3000,2048,0,4096);

	mMyCevt = new MyCellEvt();	
//	mMyCevt = 0;
	if(!InitTree(mMyCevt)){
		cout<<"ERROR initialzing Tree from MyCellEvt"<<endl;
		exit(0);
	}

//	cout<<"Initialization for cell "<<this->GetName()<<" done..."<<endl;

}

//___________________________________________________________________
MyCellRunDep::~MyCellRunDep(){

//	cout<<"calling ~MyCellRunDep() for "<<this->GetName()<<endl;
	if(mAdc)delete mAdc;
	if(mLed)delete mLed;
	if(mLedEvt)delete mLedEvt;
	
	if(mMyCellTr){
		mMyCellTr->Delete();
	}
	if(mMyCevt)delete mMyCevt;

	mAdc 		= 0;
	mLed 		= 0;
	mLedEvt 	= 0;
	mMyCellTr 	= 0;
	mMyCevt 	= 0;

}

//___________________________________________________________________
TTree* MyCellRunDep::GetPionSample( TCut cuts = "all" ){

	if(!(strcmp((const char*)cuts,"all"))){
		cout<<"MyCellRunDep::GetPionSample : no cuts specified, returning the whole tree"<<endl;
		return mMyCellTr;				//no selection cut, return the full tree
	}

	
	TTree* FilteredTr = mMyCellTr->CloneTree(0);
	MyCellEvt* mycevt = new MyCellEvt();			//class that holds the branches
	FilteredTr->GetBranch("mycevt")->SetAddress(&mycevt);
	mMyCellTr->SetBranchAddress("mycevt",&mycevt);
	TEventList *elist = new TEventList("elist","elist",10000);
	mMyCellTr->Draw(">>elist",cuts);
	Int_t nentries = elist->GetN();

	cout<<"MyCellRunDep::GetPionSample() of object "<<this->GetName()<<" get "<<nentries<<" entries"<<endl;
	for(Int_t i = 0; i < nentries; i++){
		mMyCellTr->GetEvent(elist->GetEntry(i));
		FilteredTr->Fill();
		mycevt->Clear();
	}
	
	
	delete 	elist;
	delete 	mycevt;
	elist 	= 0;
	mycevt 	= 0;

	return FilteredTr;

//	return 0;	//for test
}

//___________________________________________________________________
TH1F* MyCellRunDep::GetAdcSpectrum(){

	return mAdc;

}

//___________________________________________________________________
TH1F* MyCellRunDep::GetLEDSpectrum(){

	return mLed;

}

//___________________________________________________________________
TH2F* MyCellRunDep::GetLEDvsEvt(){

	return mLedEvt;

}

//___________________________________________________________________
Int_t MyCellRunDep::GetRunNumber(){
	
	return mRun;

}

//___________________________________________________________________
UChar_t MyCellRunDep::GetStatusBit(){

	return mStatus;

}

//___________________________________________________________________
MyCellEvt* MyCellRunDep::GetMyCellEvt(){

	return mMyCevt;

}

//___________________________________________________________________
Int_t MyCellRunDep::SetMyCellEvt( MyCellEvt* mycevt ){
	
	mMyCevt->M 	= mycevt->M;
	mMyCevt->Rnum 	= mycevt->Rnum;
//	mMyCevt->Evt 	= mycevt->Evt;
	mMyCevt->E1 	= mycevt->E1;
	mMyCevt->E2 	= mycevt->E2;
	mMyCevt->Eta1 	= mycevt->Eta1;
	mMyCevt->Phi1 	= mycevt->Phi1;
	mMyCevt->Eta2 	= mycevt->Eta2;
	mMyCevt->Phi2 	= mycevt->Phi2;
	mMyCevt->X1 	= mycevt->X1;
	mMyCevt->Y1 	= mycevt->Y1;
	mMyCevt->X2 	= mycevt->X2;
	mMyCevt->Y2 	= mycevt->Y2;
	mMyCevt->E 	= mycevt->E;
//	mMyCevt->X 	= mycevt->X;
//	mMyCevt->Y 	= mycevt->Y;
	mMyCevt->Eta 	= mycevt->Eta;
	mMyCevt->Phi 	= mycevt->Phi;
	mMyCevt->Z 	= mycevt->Z;
	mMyCevt->C12 	= mycevt->C12;

	mMyCellTr->ResetBranchAddresses();
	mMyCellTr->GetBranch("mycevt")->SetAddress(&mMyCevt);
	
	return 1;

}

//___________________________________________________________________
Int_t MyCellRunDep::SetName( const char* name ){

	if(strncmp(name,"Cellr",5)){
		cout<<"ERROR! wrong name format"<<endl;
		cout<<"Required format: Cellr{row}_c{column}_n{nstb}_run{run}"<<endl;
		return 0;
        }

	mCellrdname = name;

	return 1;

}

//___________________________________________________________________
Int_t MyCellRunDep::SetStatusBit( UChar_t status ){

	mStatus = status;
	
	return 1;

}

//___________________________________________________________________
Int_t MyCellRunDep::SetPionSample(TTree* Trpi){

	mMyCellTr = Trpi;

	return 1;

}

//___________________________________________________________________
Int_t MyCellRunDep::InitTree( MyCellEvt* mycevt ){

	mMyCellTr = new TTree("MyCellTr","Associated Pion Sample");
	mMyCellTr->Branch("mycevt","MyCellEvt",&mycevt);

	return 1;

//	return 0;	//for test
}

//___________________________________________________________________
void MyCellRunDep::Clear( Option_t* option ){
	
	this->~MyCellRunDep();

}
