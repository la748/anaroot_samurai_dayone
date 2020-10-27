#ifndef _TARTRDC_H_
#define _TARTRDC_H_

#include "TArtDataObject.hh"

class TArtRDC : public TArtDataObject {

 public:

  TArtRDC()
    : TArtDataObject(), fRX(-9999), fRY(-9999), fRA(-9999), fRB(-9999), fCHI(-1), layer(-1)
    , fRes1(-100.), fRes2(-100.), fRes3(-100.), fRes4(-100.), fRes5(-100.), fRes6(-100.)
    , fHit1(0), fHit2(0), fHit3(0), fHit4(0), fHit5(0), fHit6(0)
    , fDrf1(-100.), fDrf2(-100.), fDrf3(-100.), fDrf4(-100.), fDrf5(-100.), fDrf6(-100.)
  {;}
    ~TArtRDC(){}

  // set reconstructed values
  void  SetRDCX(Double_t val){fRX = val;}
  void  SetRDCY(Double_t val){fRY = val;}
  void  SetRDCA(Double_t val){fRA = val;}
  void  SetRDCB(Double_t val){fRB = val;}
  void  SetCHI2(Double_t val){fCHI= val;}
  void  SetLayer(Int_t val) {layer= val;}
  Double_t  GetRDCX(){return fRX ;} 
  Double_t  GetRDCY(){return fRY ;} 
  Double_t  GetRDCA(){return fRA ;} 
  Double_t  GetRDCB(){return fRB ;} 
  Int_t  GetLayer() {return layer;}
  //void  SetMulti(Int_t val, Int_t i=0) {fHit[i]=val; }
  //void  SetRes(Double_t val, Int_t i=0){fRes[i]=val; }
  void  SetRes1(Double_t val){fRes1=val; }
  void  SetRes2(Double_t val){fRes2=val; }
  void  SetRes3(Double_t val){fRes3=val; }
  void  SetRes4(Double_t val){fRes4=val; }
  void  SetRes5(Double_t val){fRes5=val; }
  void  SetRes6(Double_t val){fRes6=val; }
  void  SetHit1(Int_t val){fHit1=val; }
  void  SetHit2(Int_t val){fHit2=val; }
  void  SetHit3(Int_t val){fHit3=val; }
  void  SetHit4(Int_t val){fHit4=val; }
  void  SetHit5(Int_t val){fHit5=val; }
  void  SetHit6(Int_t val){fHit6=val; }
  void  SetDrf1(Double_t val){fDrf1=val; }
  void  SetDrf2(Double_t val){fDrf2=val; }
  void  SetDrf3(Double_t val){fDrf3=val; }
  void  SetDrf4(Double_t val){fDrf4=val; }
  void  SetDrf5(Double_t val){fDrf5=val; }
  void  SetDrf6(Double_t val){fDrf6=val; }


 private:
  // hit id
  Int_t layer; // 1:UP 2:DW -1:No GOOD Hit
  // calibrated data
  Double_t fRX; // x position
  Double_t fRY; // y position
  Double_t fRA; // dx/dz
  Double_t fRB; // dy/dz
  Double_t fCHI; // best chi-square
  Double_t fRes1,fRes2,fRes3,fRes4,fRes5,fRes6;
  Int_t fHit1,fHit2,fHit3,fHit4,fHit5,fHit6;
  Double_t fDrf1,fDrf2,fDrf3,fDrf4,fDrf5,fDrf6;
  //Double_t fRes[7]; // Residual in each plane
  //Int_t fHit[7]; // Multiplicity of each plane
  //Double_t *fRes; // Residual in each plane
  //Int_t *fHit; // Multiplicity of each plane
  
  ClassDef(TArtRDC,1)

};
#endif
