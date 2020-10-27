#ifndef _TARTDIA_H_
#define _TARTDIA_H_

#include "TArtDataObject.hh"

class TArtDia: public TArtDataObject {

 public:

  TArtDia()
    : TArtDataObject(), layer(-1), channel(-1), fWidth(-1), fTime(-1), fTimeW(-1)
  {;}
  virtual ~TArtDia(){}

  virtual void SetLayer(Int_t val){layer = val;}
  virtual void SetPlaneID(Int_t val){id_plane = val;}
  virtual void SetCh(Int_t val){channel = val;}
  virtual Int_t GetLayer(){return layer;}
  virtual Int_t GetPlaneID(){return id_plane;}
  virtual Int_t GetCh(){return channel;}

  // function to set reco data
  virtual void  SetWidth(Int_t val){fWidth = val;}
  virtual void  SetTime(Int_t val){fTime = val;}
  virtual void  SetTimeW(Int_t val){fTimeW = val;}

  // function to get reco data
  virtual Int_t GetWidth(){return fWidth;}
  virtual Int_t GetTime(){return fTime;}
  virtual Int_t GetTimeW(){return fTimeW;}

 private:

  Int_t layer;//1-4,5-8
  Int_t id_plane;//31-34,35-38
  Int_t channel;//1:1-6,2:1,3,1-2,4,1-2

  Int_t fWidth;  //by QTC
  Int_t fTime;   //by TDC
  Int_t fTimeW;  //by QTC

  ClassDef(TArtDia,1);

};
#endif
