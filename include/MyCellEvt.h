#ifndef MyCellEvt_h
#define MyCellEvt_h

//___________________________________________________________________
//class that represents the tree structure of pion sample associated with a single cell
//Yuxi Pan  11/01/2011

class MyCellEvt : public TObject {

public:
		MyCellEvt(){};
		~MyCellEvt(){};
		Float_t M;
		Float_t E;
		Float_t E1;
		Float_t E2;
		Float_t X1;		//X coordinate of the photon(higher E) associated with the current cell
		Float_t X2;		//X coordinate of the partner photon
		Float_t Y1;
		Float_t Y2;
//		Float_t	X;		//x,y coordinates of the pion
//		Float_t	Y;	
		Float_t	Eta;		//pseudoradipity of the pion
		Float_t Phi;
		Float_t Eta1;
		Float_t Eta2;
		Float_t Phi1;
		Float_t Phi2;
		Float_t Z;
		Float_t C12;		//cos(opening angle)
		Int_t	Rnum;
//		Int_t	Evt;
//		TObjArray pClusters;	//tower cluster(s) associated with the photon pair, not necessary for now 03/12/2012

private:

ClassDef(MyCellEvt,1)

};

#endif
