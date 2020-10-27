#ifndef _TARTFADC_H_
#define _TARTFADC_H_

#include "TArtDataObject.hh"

class TArtFADC : public TArtDataObject {

 public:

  TArtFADC()
    : TArtDataObject(), fCh(-1), fLength(0)
  {fCh=-1; fLength=0;
    for(Int_t i=0;i<4096;i++) fFADC[i]=0
    ;
  }
  virtual ~TArtFADC(){}

  // function to set raw data
  virtual void  SetCh(Int_t val){fCh = val;}
  virtual Int_t GetCh(){return fCh;}
  virtual void  SetLength(Int_t val){fLength = val;}
  virtual Int_t GetLength(){return fLength;}
  virtual void  SetFADC(Int_t i, Int_t val){fFADC[i] = val;}
  virtual Int_t GetFADC(Int_t i){return fFADC[i];}

 private:

  // id
  Int_t fCh; // channel 0-3
  Int_t fLength; // sample length/ch typical 4096

  // raw data
  //Int_t *fFADC; // 
  Int_t fFADC[4096]; // 
  //std::vector<Int_t> fFADC; // 

  ClassDef(TArtFADC,1)

};
#endif
