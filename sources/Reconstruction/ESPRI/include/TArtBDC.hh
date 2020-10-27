#ifndef _TARTBDC_H_
#define _TARTBDC_H_

#include "TArtDataObject.hh"

class TArtBDC : public TArtDataObject {

 public:

  TArtBDC()
    : TArtDataObject(), fBX(-9999), fBY(-9999), fBA(-9999), fBB(-9999), fCHI(-1), layer(-1)
    , fRes1(-100.), fRes2(-100.), fRes3(-100.), fRes4(-100.), fRes5(-100.), fRes6(-100.)
    , fRes7(-100.), fRes8(-100.)
    , fHit1(0), fHit2(0), fHit3(0), fHit4(0), fHit5(0), fHit6(0)
    , fHit7(0), fHit8(0)
    , fDrf1(-100.), fDrf2(-100.), fDrf3(-100.), fDrf4(-100.), fDrf5(-100.), fDrf6(-100.)
    , fDrf7(-100.), fDrf8(-100.)
  //{}
  {for(Int_t i=0;i<8;i++){
      fRes[i]=-100; fDrf[i]=-100.; fHit[i]=0, fTDC[i]=-9999;}
  }
  ~TArtBDC()
  {}

  // set reconstructed values
  void  SetBDCX(Double_t val){fBX = val;}
  void  SetBDCY(Double_t val){fBY = val;}
  void  SetBDCA(Double_t val){fBA = val;}
  void  SetBDCB(Double_t val){fBB = val;}
  void  SetCHI2(Double_t val){fCHI= val;}
  void  SetLayer(Int_t val) {layer= val;}
  Double_t  GetBDCX(){return fBX;}
  Double_t  GetBDCY(){return fBY;}
  Double_t  GetBDCA(){return fBA;}
  Double_t  GetBDCB(){return fBB;}
  Int_t  GetLayer(){return layer;}
  void  SetHit(Int_t i, Int_t val){fHit[i]=val; }
  void  SetTDC(Int_t i, Int_t val){fTDC[i]=val; }
  void  SetRes(Int_t i, Double_t val){fRes[i]=val; }
  void  SetDrf(Int_t i, Double_t val){fDrf[i]=val; }
  void  SetRes1(Double_t val){fRes1=val; }
  void  SetRes2(Double_t val){fRes2=val; }
  void  SetRes3(Double_t val){fRes3=val; }
  void  SetRes4(Double_t val){fRes4=val; }
  void  SetRes5(Double_t val){fRes5=val; }
  void  SetRes6(Double_t val){fRes6=val; }
  void  SetRes7(Double_t val){fRes7=val; }
  void  SetRes8(Double_t val){fRes8=val; }
  void  SetHit1(Int_t val){fHit1=val; }
  void  SetHit2(Int_t val){fHit2=val; }
  void  SetHit3(Int_t val){fHit3=val; }
  void  SetHit4(Int_t val){fHit4=val; }
  void  SetHit5(Int_t val){fHit5=val; }
  void  SetHit6(Int_t val){fHit6=val; }
  void  SetHit7(Int_t val){fHit7=val; }
  void  SetHit8(Int_t val){fHit8=val; }
  void  SetDrf1(Double_t val){fDrf1=val; }
  void  SetDrf2(Double_t val){fDrf2=val; }
  void  SetDrf3(Double_t val){fDrf3=val; }
  void  SetDrf4(Double_t val){fDrf4=val; }
  void  SetDrf5(Double_t val){fDrf5=val; }
  void  SetDrf6(Double_t val){fDrf6=val; }
  void  SetDrf7(Double_t val){fDrf7=val; }
  void  SetDrf8(Double_t val){fDrf8=val; }

 private:
  // hit id
  Int_t layer; // 1:UP 2:DW -1:No GOOD Drf
  // calibrated data
  Double_t fBX; // x position
  Double_t fBY; // y position
  Double_t fBA; // dx/dz
  Double_t fBB; // dy/dz
  Double_t fCHI; // best chi-square
  Double_t fRes1,fRes2,fRes3,fRes4,fRes5,fRes6,fRes7,fRes8;
  Int_t fHit1,fHit2,fHit3,fHit4,fHit5,fHit6,fHit7,fHit8;
  Double_t fDrf1,fDrf2,fDrf3,fDrf4,fDrf5,fDrf6,fDrf7,fDrf8;
  Double_t fRes[8]; // Residual in each plane
  Double_t fDrf[8]; // Drft
  Int_t fHit[8]; // Multiplicity of each plane
  Int_t fTDC[8]; // TDC
  //std::vector<Double_t> fRes;
  
  ClassDef(TArtBDC,1)

};
#endif
