#ifndef _TARTTDCHIT_H_
#define _TARTTDCHIT_H_

#include "TArtDataObject.hh"

class TArtTDCHit : public TArtDataObject {

 public:

  //  TArtTDCHit(){fLTDC=0;fRTDC=-999999;fPosition=-9999;}
  TArtTDCHit()
    : TArtDataObject(), fLTDC(0), fRTDC(-999999)
  {;}
  virtual ~TArtTDCHit(){}

  // function to set raw data
  virtual void  SetTDC(Int_t val){fLTDC = val;}
  virtual void  SetTrailTDC(Int_t val){fRTDC = val;}
  virtual Int_t GetTDC(){return fLTDC;}
  virtual Int_t GetTrailTDC(){return fRTDC;}
 
  // set reconstructed values
  virtual void  SetGeo(Int_t val){geo = val;}
  virtual Int_t GetGeo(){return geo;}

  virtual void SetWireDirection(char *val) { anodedir=val; }
  virtual void SetWireDirection(TString str) { anodedir=str; }
  virtual void SetWireID(Int_t val) { wireid=val; }
  virtual void SetWireAngle(Double_t val) { wireang=val; }
  virtual void SetWireZPosition(Double_t val) { wirez=val; }
  virtual void SetTzero(Int_t val) { toffset=val; }
  virtual void SetLayer(Int_t val) { layer=val; }
  virtual void SetPlaneID(Int_t val) { id_plane=val; }
  virtual TString *GetWireDirection() { return &anodedir; }
  virtual Int_t GetWireID() { return wireid; }
  virtual Double_t GetWireAngle() const { return wireang; }
  virtual Double_t GetWireZPosition() const { return wirez; }
  virtual Int_t GetTzero() { return toffset; }
  virtual Int_t GetLayer() { return layer; }
  virtual Int_t GetPlaneID() { return id_plane; }



  virtual void SetHitID(Int_t val) { id_hit=val; }
  virtual Int_t GetHitID() { return id_hit; }

 private:

  // parameters
  Int_t layer; // n-th layer 1-4 for BDC&RDCx, 1-3 RDCy, 1-2 Pla
  Int_t id_plane; // plane id 1-16:BDC, 17-30:RDC 31-35 Pla
  TString anodedir; // wire direction 'X', 'Y', 'U':not used
  Double_t wireang; // wire angle
  Double_t wirez; //  wire-x position
  Int_t toffset; // TDC offset
  Int_t wireid; // wire-id 1-16 for BDC, 1-32 for RDC, 1-2 Pla
  Int_t geo; // geometry address for commons substract

  // hit id
  Int_t id_hit; // hit id in array 
  Int_t l_hit; // hit id in layer
  // raw data
  Int_t fLTDC; // leading edge tdc
  Int_t fRTDC; // trailing edge tdc

  /*
 public:
  // overriding functions for sorting based on tdc
  Bool_t IsEqual(TObject *obj) const { return fLTDC == ((TArtTDCHit*)obj)->fLTDC;} 
  Bool_t IsSortable() const { return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (fLTDC < ((TArtTDCHit*)obj)->fLTDC) return -1; 
    else if (fLTDC > ((TArtTDCHit*)obj)->fLTDC) return 1; 
    else return 0; 
  }
  */

  ClassDef(TArtTDCHit,1)

};
#endif
