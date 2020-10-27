#ifndef _TARTSILICONS_H_
#define _TARTSILICONS_H_

#include "TMath.h"
#include "TArtDataObject.hh"

class TArtSILICONS: public TArtDataObject {

 public:
  TArtSILICONS()
    : TArtDataObject(), fLayer(-1), fADC(-1), fTDC(-1), fEnergy(-90000), fTime(-90000), fTimeOffseted(-90000)
  {
    fPos[0] = NAN;
    fPos[1] = NAN;
    fPos[2] = NAN;
  }
  virtual ~TArtSILICONS(){}

  virtual void SetLayer(Int_t val){fLayer = val;}
  virtual Int_t GetLayer() const {return fLayer;}

  virtual void SetPosX(Double_t val){fPos[0] = val;}
  virtual void SetPosY(Double_t val){fPos[1] = val;}
  virtual void SetPosZ(Double_t val){fPos[2] = val;}
  //  virtual void SetPos(Double_t val[]){fPos = val;}

  virtual Double_t GetPosX(){return fPos[0];}
  virtual Double_t GetPosY(){return fPos[1];}
  virtual Double_t GetPosZ(){return fPos[2];}
  virtual Double_t* GetPos(){return fPos;}
  
  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}

  // function to get raw data
  virtual Int_t GetRawADC() const {return fADC;}
  virtual Int_t GetRawTDC() const {return fTDC;}

  // function to set calibrated data
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetTime(Double_t val){fTime = val;}
  virtual void  SetTimeOffseted(Double_t val){fTimeOffseted = val;}

  // function to get calibrated data
  virtual Double_t GetEnergy() const {return fEnergy;}
  virtual Double_t GetTime() const {return fTime;}
  virtual Double_t GetTimeOffseted() const {return fTimeOffseted;}

 private:

  Int_t fLayer;
  Double_t fPos[3];  

  // raw value
  Int_t fADC;  
  Int_t fTDC;  

  // calibrated value
  Double_t fEnergy;
  Double_t fTime;
  Double_t fTimeOffseted;

  ClassDef(TArtSILICONS,1);

};
#endif
