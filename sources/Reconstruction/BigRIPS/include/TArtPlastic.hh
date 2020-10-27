#ifndef _TARTPLASTIC_H_
#define _TARTPLASTIC_H_

#include <vector>
#include "TMath.h"
#include "TArtDataObject.hh"

class TArtPlastic : public TArtDataObject {

 public:
  TArtPlastic(){Clear();zposition=-1;zoffset=0;tcalL=0;tcalR=0;}
  virtual ~TArtPlastic(){}
  virtual void Clear(){fTLRaw=-9999;fTRRaw=-9999;fQLRaw=-9999;fQRRaw=-9999;fQTCLRawWidth=-9999;fQTCRRawWidth=-9999;fQTCLRawStart=-9999;fQTCRRawStart=-9999;fTime=-9999;fTimeL=-9999;fTimeR=-9999;fTimeLSlew=-9999;fTimeRSlew=-9999;fTimeSlew=-9999;SetDataState(0);fTLRawArray.clear();fTRRawArray.clear();}

  virtual void SetZoffset(Double_t val){zoffset = val;}
  virtual void SetZposition(Double_t val){zposition = val;}
  virtual void SetTCalLeft(Double_t val){tcalL = val;}
  virtual void SetTCalRight(Double_t val){tcalR = val;}

  virtual Double_t GetZoffset(){return zoffset;}
  virtual Double_t GetZposition(){return zposition;}
  virtual Double_t GetTCalLeft(){return tcalL;}
  virtual Double_t GetTCalRight(){return tcalR;}

  // function to set raw data
  void  SetTLRaw(Int_t val){fTLRaw = val;}
  void  SetTRRaw(Int_t val){fTRRaw = val;}
  void  SetQLRaw(Int_t val){fQLRaw = val;}
  void  SetQRRaw(Int_t val){fQRRaw = val;}

  void  SetQTCLRawStart(Int_t val){fQTCLRawStart = val;}
  void  SetQTCRRawStart(Int_t val){fQTCRRawStart = val;}
  void  SetQTCLRawWidth(Int_t val){fQTCLRawWidth = val;}
  void  SetQTCRRawWidth(Int_t val){fQTCRRawWidth = val;}

  void  AddMulHitTDCL(Int_t val){fTLRawArray.push_back(val);}
  void  AddMulHitTDCR(Int_t val){fTRRawArray.push_back(val);}

  // function to get raw data
  Int_t GetTLRaw(){return fTLRaw;}
  Int_t GetTRRaw(){return fTRRaw;}
  Int_t GetQLRaw(){return fQLRaw;}
  Int_t GetQRRaw(){return fQRRaw;}
  Double_t GetQAveRaw(){return TMath::Sqrt((Double_t)fQRRaw*(Double_t)fQLRaw);}

  Int_t GetQTCLRaw(){return GetQTCLRawWidth();}
  Int_t GetQTCRRaw(){return GetQTCRRawWidth();}
  Int_t GetQTCLRawWidth(){return fQTCLRawWidth;}
  Int_t GetQTCRRawWidth(){return fQTCRRawWidth;}
  Int_t GetQTCLRawStart(){return fQTCLRawStart;}
  Int_t GetQTCRRawStart(){return fQTCRRawStart;}
  Double_t GetQTCAveRaw(){return TMath::Sqrt((Double_t)fQTCRRawWidth*(Double_t)fQTCLRawWidth);}
 
  // function to get the information related to multihit
  Int_t GetNHitL(){return fTLRawArray.size();}
  Int_t GetNHitR(){return fTRRawArray.size();}  
  Int_t GetMulHitTDCL(int i){return fTLRawArray[i];} // this function, i=0, may not return the same value as fTLRaw
  Int_t GetMulHitTDCR(int i){return fTRRawArray[i];} // this function, i=0, may not return the same value as fTRRaw

  // get reconstructed values
  virtual void SetTime(Double_t val){fTime=val;}
  virtual void SetTimeL(Double_t val){fTimeL=val;}
  virtual void SetTimeR(Double_t val){fTimeR=val;}
  virtual void SetTimeLSlew(Double_t val){fTimeLSlew=val;}
  virtual void SetTimeRSlew(Double_t val){fTimeRSlew=val;}
  virtual void SetTimeSlew (Double_t val){fTimeSlew=val;}

  // get reconstructed values
  Double_t GetTime(){return fTime;}
  Double_t GetTimeL(){return fTimeL;}
  Double_t GetTimeR(){return fTimeR;}
  Double_t GetTimeLSlew(){return fTimeLSlew;}
  Double_t GetTimeRSlew(){return fTimeRSlew;}
  Double_t GetTimeSlew (){return fTimeSlew;}

 private:

  Double_t     zposition; // global position
  Double_t     zoffset; // offset in a fpl
  Double_t     tcalL, tcalR;

  // raw data
  Int_t fTLRaw;  
  Int_t fTRRaw;
  Int_t fQLRaw;  
  Int_t fQRRaw;
  Int_t fQTCLRawWidth; // leading edge tdc - trailing edge tdc
  Int_t fQTCRRawWidth;
  Int_t fQTCLRawStart; // leading edge tdc, trigger timing is not subtracted
  Int_t fQTCRRawStart;

  // multihit tdc for the purpose of pileup 
  std::vector <Int_t> fTLRawArray;
  std::vector <Int_t> fTRRawArray;

  // reconstructed data
  Double_t   fTime;
  Double_t   fTimeL;
  Double_t   fTimeR;
  Double_t   fTimeLSlew; // after slewing correction
  Double_t   fTimeRSlew;
  Double_t   fTimeSlew;

  ClassDef(TArtPlastic,1);

};
#endif

