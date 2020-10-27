#ifndef _TARTCATANACSI_H_
#define _TARTCATANACSI_H_

#include "TArtDataObject.hh"

class TArtCATANACsI: public TArtDataObject {

 public:

 TArtCATANACsI()
   : TArtDataObject(), fADC(-1), fTDC(-1), fTRef(-1), fX(-1), fY(-1), fZ(-1),
     fEnergy(-1), fDoppCorEnergy(-1),fTime(-1), fTimeOffseted(-1), fTDCSub(-1)
    {;}
  virtual ~TArtCATANACsI(){}
  
  virtual void SetPositionX(Double_t val){fX = val;}
  virtual void SetPositionY(Double_t val){fY = val;}
  virtual void SetPositionZ(Double_t val){fZ = val;}

  virtual Double_t GetPositionX(){return fX;}
  virtual Double_t GetPositionY(){return fY;}
  virtual Double_t GetPositionZ(){return fZ;}

  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}
  virtual void  SetRawTRef(Int_t val){fTRef = val;}
  virtual void  SetRawTDC_TRefSubtracted(Int_t val){fTDCSub = val;}

  // function to get raw data
  virtual Int_t GetRawADC(){return fADC;}
  virtual Int_t GetRawTDC(){return fTDC;}
  virtual Int_t GetRawTRef(){return fTRef;}
  virtual Int_t GetRawTDC_TRefSubtracted(){return fTDCSub;}

  // function to set reco data
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetDoppCorEnergy(Double_t val){fDoppCorEnergy = val;}
  virtual void  SetTime(Double_t val){fTime = val;}
  virtual void  SetTimeOffseted(Double_t val){fTimeOffseted = val;}

  // function to get reco data
  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetDoppCorEnergy(){return fDoppCorEnergy;}
  virtual Double_t GetTime(){return fTime;}
  virtual Double_t GetTimeOffseted(){return fTimeOffseted;}

 private:

  Int_t fADC;  
  Int_t fTDC;  
  Int_t fTRef,fTDCSub;  

  Double_t fX, fY, fZ;

  Double_t fEnergy;  
  Double_t fDoppCorEnergy;  
  Double_t fTime;  
  Double_t fTimeOffseted;

  ClassDef(TArtCATANACsI,1);

};
#endif
