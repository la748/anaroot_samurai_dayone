#ifndef _TARTDALINAI_H_
#define _TARTDALINAI_H_

#include "TArtDataObject.hh"

class TArtDALINaI: public TArtDataObject {

 public:

 TArtDALINaI()
   : TArtDataObject(), fADC(0), fTDC(0), layer(-1), theta(-1), costheta(-1),
     fEnergy(-1), fDoppCorEnergy(-1), fEnergyWithoutT(-1),
     fTime(-9999), fTimeOffseted(-1), fTimeTrueMult(-1)//, dummyEnergy(-1)
    {;}
  virtual ~TArtDALINaI(){}

  virtual void SetLayer(Int_t val){layer = val;}
  virtual void SetTheta(Double_t val){theta = val;}

  virtual Int_t  GetLayer(){return layer;}
  virtual Double_t  GetTheta(){return theta;}

  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}
  //virtual void  SetRawTRef(Int_t val){fTRef = val;}

  // function to get raw data
  virtual Int_t GetRawADC(){return fADC;}
  virtual Int_t GetRawTDC(){return fTDC;}
  //virtual Int_t GetRawTRef(){return fTRef;}

  // function to set reco data
  virtual void  SetCosTheta(Double_t val){costheta = val;}
  virtual void  SetXPos(Double_t val){fXPos = val;}
  virtual void  SetYPos(Double_t val){fYPos = val;}
  virtual void  SetZPos(Double_t val){fZPos = val;}

  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetDoppCorEnergy(Double_t val){fDoppCorEnergy = val;}
  virtual void  SetEnergyWithoutT(Double_t val){fEnergyWithoutT = val;}
  virtual void  SetTime(Double_t val){fTime = val;}
  virtual void  SetTimeOffseted(Double_t val){fTimeOffseted = val;}
  virtual void  SetTimeTrueEnergy(Double_t val){fTimeTrueEnergy = val;}
  virtual void  SetBeta(Double_t val){beta = val;}
  virtual void  SetTimeTrueDoppCorEnergy(Double_t val){fTimeTrueDoppCorEnergy = val;}
  virtual void  SetTimeTrueDoppCorVertexEnergy(Double_t val){fTimeTrueDoppCorVertexEnergy = val;}
  virtual void  SetTimeTrueTime(Double_t val){fTimeTrueTime = val;}
  virtual void  SetTimeTrueTimeOffseted(Double_t val){fTimeTrueTimeOffseted = val;}
  virtual void  SetMultiplicity(Double_t val){fTimeTrueMult = val;} //added
  virtual void  SetAddBackEnergy(Double_t val){fDali_doppwa = val;} //added
  virtual void  SetAddBackEnergy_M1(Double_t val){fDali_doppwa_m1 = val;} //added
  virtual void  SetAddBackEnergy_M2(Double_t val){fDali_doppwa_m2 = val;} //added
  virtual void  SetAddBackEnergy_M3(Double_t val){fDali_doppwa_m3 = val;} //added
  virtual void  SetAddBackEnergy_M4(Double_t val){fDali_doppwa_m4 = val;} //added
  virtual void  SetAddBackEnergy_M5(Double_t val){fDali_doppwa_m5 = val;} //added
  virtual void  SetAddBackEnergy_M6(Double_t val){fDali_doppwa_m6 = val;} //added
  virtual void  SetAddBackEnergy_M7(Double_t val){fDali_doppwa_m7 = val;} //added
  virtual void  SetAddBackEnergy_M8(Double_t val){fDali_doppwa_m8 = val;} //added
  virtual void  SetAddBackEnergy_M9(Double_t val){fDali_doppwa_m9 = val;} //added
  virtual void  SetAddBackEnergy_M10(Double_t val){fDali_doppwa_m10 = val;} //added
  virtual void  SetAddBackEnergy_M11(Double_t val){fDali_doppwa_m11 = val;} //added
  virtual void  SetAddBackEnergy_M12(Double_t val){fDali_doppwa_m12 = val;} //added
  virtual void  SetAddBackEnergy_M13(Double_t val){fDali_doppwa_m13 = val;} //added
  virtual void  SetAddBackEnergy_M14(Double_t val){fDali_doppwa_m14 = val;} //added
  virtual void  SetAddBackEnergy_M15(Double_t val){fDali_doppwa_m15 = val;} //added
  virtual void  SetAddBackEnergy_M16(Double_t val){fDali_doppwa_m16 = val;} //added
  virtual void  SetAddBackEnergy_M17(Double_t val){fDali_doppwa_m17 = val;} //added
  virtual void  SetAddBackEnergy_M18(Double_t val){fDali_doppwa_m18 = val;} //added
  virtual void  SetAddBackEnergy_M19(Double_t val){fDali_doppwa_m19 = val;} //added
  virtual void  SetAddBackEnergy_M20(Double_t val){fDali_doppwa_m20 = val;} //added
  virtual void  SetTimeTrueDoppCorEnergy_m1(Double_t val){fTimeTrueDoppCorEnergy_m1 = val;} //added
  virtual void  SetTimeTrueDoppCorEnergy_m2(Double_t val){fTimeTrueDoppCorEnergy_m2 = val;} //added
  virtual void  SetTimeTrueDoppCorEnergy_m3(Double_t val){fTimeTrueDoppCorEnergy_m3 = val;} //added
  virtual void  SetTimeTrueDoppCorEnergy_m4(Double_t val){fTimeTrueDoppCorEnergy_m4 = val;} //added
  //virtual void  SetTimeTrueDoppCorEnergy_m1_test(Double_t val){fTimeTrueDoppCorEnergy_m1_test = val;} //added

  // function to get reco data
  virtual Double_t GetCosTheta(){return costheta;}
  virtual Double_t GetXPos(){return fXPos;}
  virtual Double_t GetYPos(){return fYPos;}
  virtual Double_t GetZPos(){return fZPos;}

  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetDoppCorEnergy(){return fDoppCorEnergy;}
  virtual Double_t GetEnergyWithoutT(){return fEnergyWithoutT;}
  virtual Double_t GetTime(){return fTime;}
  virtual Double_t GetTimeOffseted(){return fTimeOffseted;}
  virtual Double_t GetTimeTrueEnergy(){return fTimeTrueEnergy;}
  virtual Double_t GetBeta(){return beta;}
  virtual Double_t GetTimeTrueDoppCorEnergy(){return fTimeTrueDoppCorEnergy;}
  virtual Double_t GetTimeTrueDoppCorVertexEnergy(){return fTimeTrueDoppCorVertexEnergy;}
  virtual Double_t GetTimeTrueTime(){return fTimeTrueTime;}
  virtual Double_t GetTimeTrueTimeOffseted(){return fTimeTrueTimeOffseted;}
  virtual Double_t GetMultiplicity(){return fTimeTrueMult;} //added
  virtual Double_t GetAddBackEnergy(){return fDali_doppwa;} //added
  virtual Double_t GetAddBackEnergy_M1(){return fDali_doppwa_m1;} //added
  virtual Double_t GetAddBackEnergy_M2(){return fDali_doppwa_m2;} //added
  virtual Double_t GetAddBackEnergy_M3(){return fDali_doppwa_m3;} //added
  virtual Double_t GetAddBackEnergy_M4(){return fDali_doppwa_m4;} //added
  virtual Double_t GetAddBackEnergy_M5(){return fDali_doppwa_m5;} //added
  virtual Double_t GetAddBackEnergy_M6(){return fDali_doppwa_m6;} //added
  virtual Double_t GetAddBackEnergy_M7(){return fDali_doppwa_m7;} //added
  virtual Double_t GetAddBackEnergy_M8(){return fDali_doppwa_m8;} //added
  virtual Double_t GetAddBackEnergy_M9(){return fDali_doppwa_m9;} //added
  virtual Double_t GetAddBackEnergy_M10(){return fDali_doppwa_m10;} //added
  virtual Double_t GetAddBackEnergy_M11(){return fDali_doppwa_m11;} //added
  virtual Double_t GetAddBackEnergy_M12(){return fDali_doppwa_m12;} //added
  virtual Double_t GetAddBackEnergy_M13(){return fDali_doppwa_m13;} //added
  virtual Double_t GetAddBackEnergy_M14(){return fDali_doppwa_m14;} //added
  virtual Double_t GetAddBackEnergy_M15(){return fDali_doppwa_m15;} //added
  virtual Double_t GetAddBackEnergy_M16(){return fDali_doppwa_m16;} //added
  virtual Double_t GetAddBackEnergy_M17(){return fDali_doppwa_m17;} //added
  virtual Double_t GetAddBackEnergy_M18(){return fDali_doppwa_m18;} //added
  virtual Double_t GetAddBackEnergy_M19(){return fDali_doppwa_m19;} //added
  virtual Double_t GetAddBackEnergy_M20(){return fDali_doppwa_m20;} //added
  virtual Double_t GetTimeTrueDoppCorEnergy_m1(){return fTimeTrueDoppCorEnergy_m1;} //added
  virtual Double_t GetTimeTrueDoppCorEnergy_m2(){return fTimeTrueDoppCorEnergy_m2;} //added
  virtual Double_t GetTimeTrueDoppCorEnergy_m3(){return fTimeTrueDoppCorEnergy_m3;} //added
  virtual Double_t GetTimeTrueDoppCorEnergy_m4(){return fTimeTrueDoppCorEnergy_m4;} //added
  //virtual Double_t GetTimeTrueDoppCorEnergy_m1_test(){return fTimeTrueDoppCorEnergy_m1_test;} //added

 private:

  Int_t fADC;  
  Int_t fTDC;  
  //Int_t fTRef;

  Int_t layer;  
  Double_t theta; // angle w.r.t. target
  Double_t fXPos;
  Double_t fYPos;
  Double_t fZPos;

  Double_t costheta; // cos angle of nai
  Double_t fEnergy;
  Double_t fDoppCorEnergy;
  Double_t fEnergyWithoutT;
  Double_t fTime;
  Double_t fTimeOffseted;
  Double_t fTimeTrueEnergy;
  Double_t beta;
  Double_t fTimeTrueDoppCorEnergy;
  Double_t fTimeTrueDoppCorEnergy_m1; //added
  Double_t fTimeTrueDoppCorEnergy_m2; //added
  Double_t fTimeTrueDoppCorEnergy_m3; //added
  Double_t fTimeTrueDoppCorEnergy_m4; //added
  Double_t fTimeTrueDoppCorVertexEnergy;
  Double_t fTimeTrueTime;
  Double_t fTimeTrueTimeOffseted;
  Double_t fTimeTrueMult; //added
  Double_t fDali_doppwa; //added
  Double_t fDali_doppwa_m1; //added
  Double_t fDali_doppwa_m2; //added
  Double_t fDali_doppwa_m3; //added
  Double_t fDali_doppwa_m4; //added
  Double_t fDali_doppwa_m5; //added
  Double_t fDali_doppwa_m6; //added
  Double_t fDali_doppwa_m7; //added
  Double_t fDali_doppwa_m8; //added
  Double_t fDali_doppwa_m9; //added
  Double_t fDali_doppwa_m10; //added
  Double_t fDali_doppwa_m11; //added
  Double_t fDali_doppwa_m12; //added
  Double_t fDali_doppwa_m13; //added
  Double_t fDali_doppwa_m14; //added
  Double_t fDali_doppwa_m15; //added
  Double_t fDali_doppwa_m16; //added
  Double_t fDali_doppwa_m17; //added
  Double_t fDali_doppwa_m18; //added
  Double_t fDali_doppwa_m19; //added
  Double_t fDali_doppwa_m20; //added
  //Double_t fTimeTrueDoppCorEnergy_m1_test; //added
  
  ClassDef(TArtDALINaI,1);

};
#endif
