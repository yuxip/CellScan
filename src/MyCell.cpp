#include "TObjArray.h"
#include "TTree.h"
#include "Geom.h"
#include "CalibStr.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"

#include "MyCell.h"
#include "MyCellRunDep.h"
#include "MyCellEvt.h"

ClassImp(MyCell)

MyCell::MyCell():TObject(){
	
	VecMyCellRunDep = 0;
	mGeom		= 0;
	mIterhis	= 0;
	mcevt		= 0;
	
}

//___________________________________________________________________
MyCell::MyCell( const char* name , Int_t nstb, Int_t row, Int_t col,
		CalibStr* gain,	CalibStr* gcor, Geom* p_geom ){
	
//	cout<<"initializing cell with name: "<<name<<endl;
	if(strncmp(name,"Cellr",5)){
		cout<<"MyCell::MyCell : wrong name format "<<endl;
		cout<<"Required format Cellr*_c*_n*"<<endl;
		exit(0);
	}
	mCellname = name;	

	if(!Legal(2,nstb,row,col)){
		cout<<"illegal (nstb1,row0,col0) combination"<<endl;
		exit(0);
	}
	
	SetNstb(nstb);
	SetRow(row);
	SetCol(col);
		
	if(!SetGain(gain)){
		cout<<"Error setting gain"<<endl;
		exit(0);
	}
	if(!SetGainCor(gcor)){
		cout<<"Error setting gain correction"<<endl;
		exit(0);
	}
	
	Float_t Cellgeom[2];
	SetGeom(p_geom);
	Cellgeom[0] =  (mGeom->FpdTowWid(2,nstb)[0]);
	Cellgeom[1] =  (mGeom->FpdTowWid(2,nstb)[1]);
//	cout<<"Cellgeom[0]: "<<Cellgeom[0]<<" Cellgeom[1]: "<<Cellgeom[1]<<endl;
	SetCellWidthXY(Cellgeom[0], Cellgeom[1]);			//horizontal ,vertical width
	
	Float_t xLocal          = (this->GetCellWidthX())*(col + .5);	//row, col both start from 0
        Float_t yLocal          = (this->GetCellWidthY())*(row + .5);
        Float_t z       	= 0.;

        TVector3      		loctow(xLocal,yLocal,z);
        TVector3 vtow   	= mGeom->GlobalXYZ(2,nstb,loctow);
        Float_t eta		= vtow.PseudoRapidity();
        Float_t phi		= vtow.Phi();
	SetCellEtaPhi(eta, phi);
/*
	cout<<"debug:	Cell name:		"	<<mCellname<<endl; 
	cout<<"		Cell gain:		"	<<this->GetGain()<<endl;
	cout<<"		Cell gaincor:		"	<<this->GetGainCor()<<endl; 		
	cout<<"		Cell geometry: x-width: "	<<this->GetCellWidthX()<<"  y-width: "<<this->GetCellWidthY()<<endl;
	cout<<"		Cell pseudorapidity:	"	<<this->GetEta()<<endl;
	cout<<"		Cell azimuth:		"	<<this->GetPhi()<<endl;
*/
	VecMyCellRunDep = new TObjArray(10,0);
	//VecMyCellRunDep->SetOwner(1);
	mIterhis	= 0;
	MyCellEvt* mcevt = new MyCellEvt();				//for merging trees
	

/*--------------------The following will go into MyCellRunDep objects-------------------------/
	Char_t adcname[30];
	sprintf(adcname,"hadc_%s",name);
	adc = new TH1F(adcname,adcname,4096,0,4096);
	Char_t ledname[30];
	sprintf(ledname,"hled_%s",name);
	adc = new TH1F(ledname,ledname,4096,0,4096);
	Char_t treename[30];
	sprintf(treename,"Tr_%s",name);
	pt = new TTree(treename,treename,99);
----------------------------------------------------------------------------------------------*/

//	cout<<"MyCell object "<<mCellname<<" initialization done..."<<endl;

}

//___________________________________________________________________
MyCell::~MyCell(){
	
//	cout<<"calling ~MyCell() for "<<this->GetName()<<endl;
	//delete mIterhis;
//	cout<<"about to call VecMyCellRunDep::Delete() for "<<this->GetName()<<endl;
	VecMyCellRunDep->Delete();
	delete VecMyCellRunDep;
//	delete mcevt;
	mcevt = 0;
	VecMyCellRunDep	= 0;
	mIterhis 	= 0;
	mGeom	 	= 0;
	VecMyCellRunDep = 0;

}

//___________________________________________________________________
Int_t MyCell::GetNstb(){
	
	return mNstb;

}

//___________________________________________________________________
Int_t MyCell::GetRow(){
	
	return mRow;

}

//___________________________________________________________________
Int_t MyCell::GetCol(){
	
	return mCol;

}

//___________________________________________________________________
Float_t MyCell::GetEta(){

	return mEta;

}

//___________________________________________________________________
Float_t MyCell::GetPhi(){

	return mPhi;

}

//___________________________________________________________________
Float_t MyCell::GetCellWidthX(){

	return mWidthx;

}

//___________________________________________________________________
Float_t MyCell::GetCellWidthY(){

	return mWidthy;

}

//___________________________________________________________________
TTree* MyCell::GetPionSample( TCut cuts = "all", char* runlist = "cellrun.list" ){

//From the pion samples held by each MyCellRunDep object, assemble a new tree (with the same structure) according to "cuts" 
	Int_t totalevents = 0;
	fstream inFile(runlist,ios::in);
	if(!inFile){
		cout<<"MyCell::GetPionSample : ERROR opening "<<runlist<<endl;
		exit(0);
	}
	
	TTree* MergedTr = new TTree("MyCellTr","Associated Pion Sample");
	MergedTr->Branch("mycevt","MyCellEvt",&mcevt);
	TList* treelist = new TList();
	Long_t run;
	while(inFile>>run){
		cout<<run<<" added..."<<endl;
		if(!this->GetMyCellrd(run)){
			cout<<"In MyCell::GetPionSample() : ERROR getting MyCellRunDep "<<run<<" object!!"<<endl;
			exit(0);
		}
		
		TTree* tr = this->GetMyCellrd(run)->GetPionSample(cuts);
		treelist->Add(tr);
		totalevents += (Int_t)tr->GetEntries();
	}
	cout<<"treelist nentries: "<<treelist->GetEntries()<<endl;
	MergedTr->Merge(treelist);
	//MergedTr->Print();
	cout<<"MyCell::GetPionSample() : # entries returned: "<<totalevents<<endl;
	
	delete treelist;
	treelist = 0;
	return MergedTr;
		
}

//___________________________________________________________________
TTree* MyCell::GetPionSample( TCut cuts = "all", Long_t runnum = 12098007 ){
	
	if(!this->GetMyCellrd(runnum)){
		cout<<"In MyCell::GetPionSample() : ERROR getting MyCellRunDep "<<runnum<<" object!!"<<endl;
		exit(0);
	}

	TTree* tr = this->GetMyCellrd(runnum)->GetPionSample(cuts);
	return tr;

}

//___________________________________________________________________
MyCellRunDep* MyCell::GetMyCellrd( Long_t runnum ){

	//cout<<"retrieving MyCellRunDep object of "<<this->GetName()<<" at run_"<<runnum<<endl;

	Char_t objname[50];
	sprintf(objname, "Cellr%d_c%d_n%d_run%ld",mRow,mCol,mNstb,runnum);
	MyCellRunDep* myCellrd = (MyCellRunDep*)VecMyCellRunDep->FindObject(objname);
	if(!myCellrd){
//		cout<<"=====in GetMyCellrd()====="<<endl;
//		cout<<objname<<" does not exist..."<<endl;
		return 0;
	}
	
	return myCellrd;

}

//___________________________________________________________________
TString MyCell::GetPartnerByXY( Float_t X2, Float_t Y2, Long_t runnum ){

	TVector3 gvec(X2,Y2,0);		//in STAR coordinate system. doesn't matter where Z is
	Int_t nstb = mGeom->getNSTB(X2,Y2);
	if(!nstb){
		cout<<"MyCell::GetPartnerByXY() : ERROR getting nstb number!!"<<endl;
		exit(0);
	}

	TVector3 lvec = mGeom->LocalXYZ(2,nstb,gvec,true);
	Int_t rpar = lvec.Y();
	Int_t cpar = lvec.X();
	Char_t partner[20];
	sprintf(partner,"Cellr%d_c%d_n%d_run%ld",rpar,cpar,nstb,runnum);
	TString spartner(partner);
	
	return spartner;

}	

//___________________________________________________________________
TString MyCell::GetPartnerByEtaPhi( Float_t Eta2, Float_t Phi2, Long_t runnum ){

	Float_t theta = 2*atan(TMath::Exp(-Eta2));
	Float_t zz = *(mGeom->ZFPD(2,3));	//take small cell zoffset temporarily
	Float_t pt = zz*tan(theta); 
	TVector3 vec;
	vec.SetPtEtaPhi(pt,Eta2,Phi2);
	TString spartner = GetPartnerByXY( vec.X(),vec.Y(),runnum );

	return spartner;

}

//___________________________________________________________________
TH1F* MyCell::GetAdcSpectrum( Long_t runnum ){

	//cout<<"retrieving ADC spectrum of "<<this->GetName()<<" at run_"<<runnum<<endl;

	Char_t objname[50];
        sprintf(objname, "Cellr%d_c%d_n%d_run%ld",mRow,mCol,mNstb,runnum);
	MyCellRunDep* myCellrd = this->GetMyCellrd(runnum);
	if(!myCellrd){
		cout<<"=====in GetAdcSpecturm()====="<<endl;
		cout<<"adc of "<<objname<<" does not exist..."<<endl;
		return 0;
	}
	
	return myCellrd->GetAdcSpectrum();

}

//___________________________________________________________________
TH1F* MyCell::GetLEDSpectrum( Long_t runnum ){

	//cout<<"retrieving LED spectrum of "<<this->GetName()<<" at run_"<<runnum<<endl;

	Char_t objname[50];
	sprintf(objname, "myCellrd_%ld", runnum);
	MyCellRunDep* myCellrd = this->GetMyCellrd(runnum);
	if(!myCellrd){
		cout<<"=====in GetLEDSpectrum====="<<endl;
		cout<<"LED of "<<objname<<" does not exist..."<<endl;
		return 0;
	}
	
	return myCellrd->GetLEDSpectrum();

}

//___________________________________________________________________
TH2F* MyCell::GetLEDvsEvt( Long_t runnum ){

	//cout<<"retrieving LED vs Event# of "<<this->GetName()<<" at run_"<<runnum<<endl;

	Char_t objname[50];
	sprintf(objname, "myCellrd_%ld", runnum);
	MyCellRunDep* myCellrd = this->GetMyCellrd(runnum);
	if(!myCellrd){
		cout<<"=====in GetLEDvsEvt====="<<endl;
		cout<<objname<<" does not exist..."<<endl;
		return 0;
	}
	
	return myCellrd->GetLEDvsEvt();

}

//___________________________________________________________________
Int_t MyCell::GetNth( Long_t runnum, Float_t threshold ){
	
	if((threshold<0)||(threshold>4095)){
		cout<<threshold<<" is an invalid threshold"<<endl;
		return -1;
	}

	TH1F* adc = this->GetAdcSpectrum(runnum);
	if(!adc){
		cout<<"ERROR in GetNth()"<<endl;
		return -2;
	}
	Int_t thbin = adc->FindBin(threshold);
	Int_t Nth = adc->Integral(thbin,4096);

	return Nth;

}

//___________________________________________________________________
UChar_t MyCell::GetStatusBit( Long_t runnum ){
	
	cout<<"retrieving status bit of "<<this->GetName()<<" at run_"<<runnum<<endl;

        Char_t objname[50];
        sprintf(objname, "%s_%ld", this->GetName(),runnum);
        MyCellRunDep* myCellrd = this->GetMyCellrd(runnum);
        if(!myCellrd){
                cout<<"=====in GetStatusBit()====="<<endl;
                cout<<"ERROR getting status bit of"<<objname<<endl;
                return 0;
        }

        return myCellrd->GetStatusBit();

}

//___________________________________________________________________
Float_t MyCell::GetGain(){
	
	return mGain;

}

//___________________________________________________________________
Float_t MyCell::GetGainCor(){
	
	return mGcor;

}

//___________________________________________________________________
void MyCell::Clear( Option_t* option ){
	
	this->~MyCell();

}

//___________________________________________________________________
Int_t MyCell::InitMyCellRunDep( Long_t runnum ){

	Int_t 	nstb	= this->GetNstb(); 
	Int_t 	row0	= this->GetRow();
	Int_t 	col0	= this->GetCol();
	
	Char_t 	mcrdname[100];
	sprintf(mcrdname,"Cellr%d_c%d_n%d_run%ld",row0,col0,nstb,runnum);
//	cout<<"MyCell::InitMyCellRunDep, checking "<<mcrdname<<endl;
	if(this->GetMyCellrd(runnum)){
//		cout<<mcrdname<<" already exists!! abort..."<<endl;
	}
	else{
		MyCellRunDep* mcrd = new MyCellRunDep(mcrdname);      //for each run, create a MyCellRunDep object for each cell
		this->InsertMyCellRunDep(mcrd);
	}

	return 1;
}
	
//___________________________________________________________________
Bool_t MyCell::Legal( Int_t iew, Int_t nstb, Int_t row0, Int_t col0 ){
	
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

//___________________________________________________________________
Int_t MyCell::SetNstb( Int_t nstb ){

	if((nstb<1)||(nstb>4)){
		cout<<"SetNstb(): illegal nstb number!"<<endl;
		return 0;
	}
	mNstb = nstb;

	return 1;

}	

//___________________________________________________________________
Int_t MyCell::SetRow( Int_t row ){
	
	mRow = row;

	return 1;

}

//___________________________________________________________________
Int_t MyCell::SetCol( Int_t col ){
	
	mCol = col;
	
	return 1;

}

//___________________________________________________________________
Int_t MyCell::SetCellWidthXY( Float_t wx, Float_t wy ){
	
	mWidthx = wx;
	mWidthy = wy;

	return 1;

}

//___________________________________________________________________
Int_t MyCell::SetCellEtaPhi( Float_t eta, Float_t phi ){
	
	mEta = eta;
	mPhi = phi;

	return 1;

}

//___________________________________________________________________
Int_t MyCell::SetGain( CalibStr* gain ){

	Int_t nstb	= this->GetNstb(); 
	Int_t row	= this->GetRow();
	Int_t col	= this->GetCol();

	if(!Legal(2,nstb,row,col)){
		cout<<"ERROR in SetGain()"<<endl;
		cout<<"wrong (nstb,row,col) combination"<<endl;
		return 0;
	}

	mGain = gain->GetValue(2,nstb,row,col);	//nstb starts from 1. row,col start from 0

	return 1;

}

//___________________________________________________________________
Int_t MyCell::SetGainCor( CalibStr* gcor ){

	Int_t nstb	= this->GetNstb(); 
	Int_t row	= this->GetRow();
	Int_t col	= this->GetCol();

	if(!Legal(2,nstb,row,col)){
		cout<<"ERROR in SetGainCor()"<<endl;
		cout<<"wrong (nstb,row,col) combination"<<endl;
		return 0;
	}

	mGcor = gcor->GetValue(2,nstb,row,col);	//nstb starts from 1. row,col start from 0

	return 1;

}

//___________________________________________________________________
Int_t MyCell::SetGeom( Geom* geom ){

	mGeom = geom;
	return 1;
}

//___________________________________________________________________
Int_t MyCell::SetStatusBit( Long_t runnum, UChar_t status ){

	cout<<"setting status bit of "<<this->GetName()<<" at run_"<<runnum<<endl;

        Char_t objname[50];
        sprintf(objname, "%s_%ld",this->GetName(),runnum);
        MyCellRunDep* myCellrd = this->GetMyCellrd(runnum);
        if(!myCellrd){
                cout<<"=====in SetStatusBit()====="<<endl;
                cout<<"ERROR setting status bit of"<<objname<<endl;
                return 0;
        }

        myCellrd->SetStatusBit(status);
	
	return 1;

}

//___________________________________________________________________
Int_t MyCell::InsertMyCellRunDep( MyCellRunDep* mCellrun ){

	VecMyCellRunDep->Add(mCellrun);

	return 1;

}
