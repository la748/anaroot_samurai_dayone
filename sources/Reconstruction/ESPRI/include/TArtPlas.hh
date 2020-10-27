#ifndef _TARTPLAS_H_
#define _TARTPLAS_H_

#include "TArtDataObject.hh"

class TArtPlas: public TArtDataObject {

 public:

  TArtPlas()
    : TArtDataObject(), fADC(0), fTDC(0), layer(-1), channel(-1), 
      fEnergy(-1), fWidth(-1), fTime(-1), fTimeW(-1)
  {;}
  virtual ~TArtPlas(){}

  virtual void SetLayer(Int_t val){layer = val;}
  virtual Int_t  GetLayer(){return layer;}
  virtual void SetPlaneID(Int_t val){id_plane = val;}
  virtual Int_t  GetPlaneID(){return id_plane;}
  virtual void SetCh(Int_t val){channel = val;}
  virtual Int_t  GetCh(){return channel;}

  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}

  // function to get raw data
  virtual Int_t GetRawADC(){return fADC;}
  virtual Int_t GetRawTDC(){return fTDC;}

  // function to set reco data
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetWidth(Double_t val){fWidth = val;}
  virtual void  SetTime(Double_t val){fTime = val;}
  virtual void  SetTimeW(Double_t val){fTimeW = val;}

  // function to get reco data
  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetWidth(){return fWidth;}
  virtual Double_t GetTime(){return fTime;}
  virtual Double_t GetTimeW(){return fTimeW;}

 private:

  Int_t fADC;  
  Int_t fTDC;  

  Int_t layer;//1-4,5-8
  Int_t id_plane;//31-34,35-38
  Int_t channel;//1:1-6,2:1,3,1-2,4,1-2

  Double_t fEnergy; //by QDC
  Double_t fWidth;  //by QTC
  Double_t fTime;   //by TDC
  Double_t fTimeW;  //by QTC

  ClassDef(TArtPlas,1);

};
#endif
