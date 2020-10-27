#ifndef _TARTHIMEPLA_H_
#define _TARTHIMEPLA_H_

#include "TString.h"

#include <iostream>

#include "TArtDataObject.hh"

class TArtHIMEPla : public TArtDataObject
{
public:
  TArtHIMEPla()
    : TArtDataObject(),
      fLayer(-1), fSubLayer(-1), fHit(0),
      fQAvePed(-1), fQAveCal(-1), fLogQPed(0), fLogQCal(0),
      fIvSqrtQAvePed(-1), 
      fDTRaw(-1), fDTCal(-1), fDTSlw(-1), fTAveRaw(-1), fTAveCal(-1), fTAveSlw(-1),
      fTAveCalT0(-1), fTAveSlwT0(-1),
      fTTOFGamma(-90000), fTTOFNeutron(-90000),
      fPosCal(0), fPosSlw(0), fFlightLength(-1), fFlightAngle(-1)
  {
    for(Int_t i=0; i<2; ++i) {
      fQRaw[i] = -1 ;
      fQCal[i] = -1 ;
      fQPed[i] = -1 ;
      fTRaw[i] = -1 ;
      fTCal[i] = -1 ;
      fTSlw[i] = -1 ;
      fIvSqrtQPed[i] = -1;
      fTSlwT0[i] = -1;
      fTCalT0[i] = -1;
      fTRawTRef[i] = -1;
      fTCalTRef[i] = -1;
      fTRawTSub[i] = -9999999;
      fTCalTSub[i] = -9999999;
    }
    for(Int_t i=0; i<3; ++i) {
      fDetPos[i] = -90000;
      fPos[i] = -90000;
    }
  }
  virtual ~TArtHIMEPla(){;}

  enum DetDirectionDef { Vert, Horiz};

  virtual void SetLayer(Int_t val){fLayer = val;}
  virtual void SetSubLayer(Int_t val){fSubLayer = val;}
  virtual void SetDetDirect(DetDirectionDef val){fDetDirect = val;}

  virtual void SetDetPos(const Double_t* val){for(Int_t i=0; i<3; ++i){fDetPos[i] = val[i];}}
  virtual void SetDetPos(Double_t val, Int_t i){fDetPos[i] = val;}
  virtual void SetHit(Int_t val){fHit = val;}

  // setters for charge : both computer and human friendly
  virtual void SetQRaw      (UInt_t i, Int_t    val){fQRaw[i] = val;} 
  virtual void SetQPed      (UInt_t i, Double_t val){fQPed[i] = val;}
  virtual void SetQCal      (UInt_t i, Double_t val){fQCal[i] = val;}
  virtual void SetIvSqrtQPed(UInt_t i, Double_t val){fIvSqrtQPed[i] = val;}

  // Up <-> 0 and Down <-> 1
  virtual void SetQURaw      (Int_t    val){SetQRaw(0,val);} 
  virtual void SetQDRaw      (Int_t    val){SetQRaw(1,val);}
  virtual void SetQUPed      (Double_t val){SetQPed(0,val);}
  virtual void SetQDPed      (Double_t val){SetQPed(1,val);}
  virtual void SetQUCal      (Double_t val){SetQCal(0,val);}
  virtual void SetQDCal      (Double_t val){SetQCal(1,val);}
  virtual void SetIvSqrtQUPed(Double_t val){SetIvSqrtQPed(0,val);}
  virtual void SetIvSqrtQDPed(Double_t val){SetIvSqrtQPed(1,val);}

  // Left <-> 0 and Right <-> 1
  virtual void SetQLRaw      (Int_t    val){SetQRaw(0,val);} 
  virtual void SetQRRaw      (Int_t    val){SetQRaw(1,val);}
  virtual void SetQLPed      (Double_t val){SetQPed(0,val);}
  virtual void SetQRPed      (Double_t val){SetQPed(1,val);}
  virtual void SetQLCal      (Double_t val){SetQCal(0,val);}
  virtual void SetQRCal      (Double_t val){SetQCal(1,val);}
  virtual void SetIvSqrtQLPed(Double_t val){SetIvSqrtQPed(0,val);}
  virtual void SetIvSqrtQRPed(Double_t val){SetIvSqrtQPed(1,val);}

  virtual void SetQAvePed(Double_t val){fQAvePed = val;}
  virtual void SetQAveCal(Double_t val){fQAveCal = val;}
  virtual void SetLogQPed(Double_t val){fLogQPed = val;}
  virtual void SetLogQCal(Double_t val){fLogQCal = val;}
  virtual void SetIvSqrtQAvePed(Double_t val){fIvSqrtQAvePed = val;}

  // setters for time : both computer and human friendly
  virtual void SetTRaw  (UInt_t i, Int_t    val){fTRaw[i] = val;}
  virtual void SetTCal  (UInt_t i, Double_t val){fTCal[i] = val;}
  virtual void SetTSlw  (UInt_t i, Double_t val){fTSlw[i] = val;}
  virtual void SetTCalT0(UInt_t i, Double_t val){fTCalT0[i] = val;}
  virtual void SetTSlwT0(UInt_t i, Double_t val){fTSlwT0[i] = val;}

  //for v1290 s020
  virtual void SetTRawTRef (UInt_t i, Int_t    val){fTRawTRef[i] = val;}
  virtual void SetTRawTSub  (UInt_t i, Int_t    val){fTRawTSub[i] = val;}
  virtual void SetTCalTRef (UInt_t i, Int_t    val){fTCalTRef[i] = val;}
  virtual void SetTCalTSub  (UInt_t i, Int_t    val){fTCalTSub[i] = val;}

  
  // Up <-> 0 and Down <-> 1
  virtual void SetTURaw  (Int_t    val){SetTRaw(0,val);}  
  virtual void SetTDRaw  (Int_t    val){SetTRaw(1,val);}  
  virtual void SetTUCal  (Double_t val){SetTCal(0,val);}  
  virtual void SetTDCal  (Double_t val){SetTCal(1,val);}  
  virtual void SetTUSlw  (Double_t val){SetTSlw(0,val);}  
  virtual void SetTDSlw  (Double_t val){SetTSlw(1,val);}  
  virtual void SetTUCalT0(Double_t val){SetTCalT0(0,val);}
  virtual void SetTDCalT0(Double_t val){SetTCalT0(1,val);}
  virtual void SetTUSlwT0(Double_t val){SetTSlwT0(0,val);}
  virtual void SetTDSlwT0(Double_t val){SetTSlwT0(1,val);}
  
  //for v1290 s020
  virtual void SetTURawTRef  (Int_t    val){SetTRawTRef(0,val);}  
  virtual void SetTDRawTRef  (Int_t    val){SetTRawTRef(1,val);}
  virtual void SetTUCalTRef  (Int_t    val){SetTRawTRef(0,val);}  
  virtual void SetTDCalTRef  (Int_t    val){SetTRawTRef(1,val);}
  virtual void SetTURawTSub  (Int_t    val){SetTRawTSub(0,val);}  
  virtual void SetTDRawTSub  (Int_t    val){SetTRawTSub(1,val);}  
  virtual void SetTUCalTSub  (Double_t val){SetTCalTSub(0,val);}  
  virtual void SetTDCalTSub  (Double_t val){SetTCalTSub(1,val);}  
  // virtual void SetTUSlwSub  (Double_t val){SetTSlw(0,val);}  
  // virtual void SetTDSlwSub  (Double_t val){SetTSlw(1,val);}  

  // Left <-> 0 and Right <-> 1
  virtual void SetTLRaw  (Int_t    val){SetTRaw(0,val);}  
  virtual void SetTRRaw  (Int_t    val){SetTRaw(1,val);}  
  virtual void SetTLCal  (Double_t val){SetTCal(0,val);}  
  virtual void SetTRCal  (Double_t val){SetTCal(1,val);}  
  virtual void SetTLSlw  (Double_t val){SetTSlw(0,val);}  
  virtual void SetTRSlw  (Double_t val){SetTSlw(1,val);}  
  virtual void SetTLCalT0(Double_t val){SetTCalT0(0,val);}
  virtual void SetTRCalT0(Double_t val){SetTCalT0(1,val);}
  virtual void SetTLSlwT0(Double_t val){SetTSlwT0(0,val);}
  virtual void SetTRSlwT0(Double_t val){SetTSlwT0(1,val);}

  //for v1290 s021
  virtual void SetTLRawTRef  (Int_t    val){SetTRawTRef(0,val);}  
  virtual void SetTRRawTRef  (Int_t    val){SetTRawTRef(1,val);}
  virtual void SetTLCalTRef  (Int_t    val){SetTCalTRef(0,val);}  
  virtual void SetTRCalTRef  (Int_t    val){SetTCalTRef(1,val);}
  virtual void SetTLRawTSub  (Int_t    val){SetTRawTSub(0,val);}  
  virtual void SetTRRawTSub  (Int_t    val){SetTRawTSub(1,val);}  
  virtual void SetTLCalTSub  (Double_t val){SetTCalTSub(0,val);}  
  virtual void SetTRCalTSub  (Double_t val){SetTCalTSub(1,val);}  
  // virtual void SetTLSlwSub  (Double_t val){SetTSlw(0,val);}  
  // virtual void SetTRSlwSub  (Double_t val){SetTSlw(1,val);}  



  virtual void SetDTRaw(Double_t val){fDTRaw = val;}
  virtual void SetDTCal(Double_t val){fDTCal = val;}
  virtual void SetDTSlw(Double_t val){fDTSlw = val;}
  virtual void SetTAveRaw(Double_t val){fTAveRaw = val;}
  virtual void SetTAveCal(Double_t val){fTAveCal = val;}
  virtual void SetTAveSlw(Double_t val){fTAveSlw = val;}

  virtual void SetTAveCalT0(Double_t val){fTAveCalT0 = val;}
  virtual void SetTAveSlwT0(Double_t val){fTAveSlwT0 = val;}
  virtual void SetTTOFGamma(Double_t val){fTTOFGamma = val;}
  virtual void SetTTOFNeutron(Double_t val){fTTOFNeutron = val;}

  virtual void SetPosCal(Double_t val){fPosCal = val;}
  virtual void SetPosSlw(Double_t val){fPosSlw = val;}
  virtual void SetPos(const Double_t* val){for(Int_t i=0; i<3; ++i) fPos[i] = val[i];}
  virtual void SetPos(Double_t val, Int_t i){fPos[i] = val;}
  virtual void SetFlightLength(Double_t val){fFlightLength = val;}
  virtual void SetFlightAngle(Double_t val){fFlightAngle = val;}

  virtual Int_t GetLayer() const {return fLayer;}
  virtual Int_t GetSubLayer() const {return fSubLayer;}
  virtual DetDirectionDef GetDetDirect() const {return fDetDirect;}
  virtual Bool_t DirectionIs(DetDirectionDef val) const {return (fDetDirect==val);}

  virtual const Double_t* GetDetPos() const {return fDetPos;}
  virtual Double_t GetDetPos(Int_t i) const {return fDetPos[i];}
  virtual Int_t GetHit() const {return fHit;}

  // getters for charge : both computer and human friendly
  virtual Int_t    GetQRaw      (UInt_t i) const {return fQRaw[i] ;} 
  virtual Double_t GetQPed      (UInt_t i) const {return fQPed[i] ;}
  virtual Double_t GetQCal      (UInt_t i) const {return fQCal[i] ;}
  virtual Double_t GetIvSqrtQPed(UInt_t i) const {return fIvSqrtQPed[i] ;}

  // Up <-> 0 and Down <-> 1
  virtual Int_t    GetQURaw()       const {return GetQRaw(0);} 
  virtual Int_t    GetQDRaw()       const {return GetQRaw(1);}
  virtual Double_t GetQUPed()       const {return GetQPed(0);}
  virtual Double_t GetQDPed()       const {return GetQPed(1);}
  virtual Double_t GetQUCal()       const {return GetQCal(0);}
  virtual Double_t GetQDCal()       const {return GetQCal(1);}
  virtual Double_t GetIvSqrtQUPed() const {return GetIvSqrtQPed(0);}
  virtual Double_t GetIvSqrtQDPed() const {return GetIvSqrtQPed(1);}

  // Left <-> 0 and Right <-> 1
  virtual Int_t    GetQLRaw() const {return GetQRaw(0);} 
  virtual Int_t    GetQRRaw() const {return GetQRaw(1);}
  virtual Double_t GetQLPed() const {return GetQPed(0);}
  virtual Double_t GetQRPed() const {return GetQPed(1);}
  virtual Double_t GetQLCal() const {return GetQCal(0);}
  virtual Double_t GetQRCal() const {return GetQCal(1);}
  virtual Double_t GetIvSqrtQLPed() const {return GetIvSqrtQPed(0);}
  virtual Double_t GetIvSqrtQRPed() const {return GetIvSqrtQPed(1);}

  virtual Double_t GetQAvePed() const {return fQAvePed;}
  virtual Double_t GetQAveCal() const {return fQAveCal;}
  virtual Double_t GetLogQPed() const {return fLogQPed;}
  virtual Double_t GetLogQCal() const {return fLogQCal;}
  virtual Double_t GetIvSqrtQAvePed() const {return fIvSqrtQAvePed;}

  // Getters for time : both computer and human friendly
  virtual Int_t    GetTRaw  (UInt_t i) const {return fTRaw[i];}
  virtual Double_t GetTCal  (UInt_t i) const {return fTCal[i];}
  virtual Double_t GetTSlw  (UInt_t i) const {return fTSlw[i];}
  virtual Double_t GetTCalT0(UInt_t i) const {return fTCalT0[i];}
  virtual Double_t GetTSlwT0(UInt_t i) const {return fTSlwT0[i];}


  //for v1290 s020
  virtual Int_t    GetTRawTRef  (UInt_t i) const {return fTRawTRef[i];}
  virtual Double_t GetTCalTRef  (UInt_t i) const {return fTCalTRef[i];}
  virtual Int_t    GetTRawTSub  (UInt_t i) const {return fTRawTSub[i];}
  virtual Double_t GetTCalTSub  (UInt_t i) const {return fTCalTSub[i];}
  
  // Up <-> 0 and Down <-> 1
  virtual Int_t    GetTURaw  () const {return GetTRaw(0);}  
  virtual Int_t    GetTDRaw  () const {return GetTRaw(1);}  
  virtual Double_t GetTUCal  () const {return GetTCal(0);}  
  virtual Double_t GetTDCal  () const {return GetTCal(1);}  
  virtual Double_t GetTUSlw  () const {return GetTSlw(0);}  
  virtual Double_t GetTDSlw  () const {return GetTSlw(1);}  
  virtual Double_t GetTUCalT0() const {return GetTCalT0(0);}
  virtual Double_t GetTDCalT0() const {return GetTCalT0(1);}
  virtual Double_t GetTUSlwT0() const {return GetTSlwT0(0);}
  virtual Double_t GetTDSlwT0() const {return GetTSlwT0(1);}
  
  //for v1290 s020
  virtual Int_t    GetTURawTRef  () const {return GetTRawTRef(0);}  
  virtual Int_t    GetTDRawTRef  () const {return GetTRawTRef(1);}  
  virtual Double_t GetTUCalTRef  () const {return GetTCalTRef(0);}  
  virtual Double_t GetTDCalTRef  () const {return GetTCalTRef(1);}  
  virtual Int_t    GetTURawTSub  () const {return GetTRawTSub(0);}  
  virtual Int_t    GetTDRawTSub  () const {return GetTRawTSub(1);}  
  virtual Double_t GetTUCalTSub  () const {return GetTCalTSub(0);}  
  virtual Double_t GetTDCalTSub  () const {return GetTCalTSub(1);}  
  

  // Left <-> 0 and Right <-> 1
  virtual Int_t    GetTLRaw  () const {return GetTRaw(0);}  
  virtual Int_t    GetTRRaw  () const {return GetTRaw(1);}  
  virtual Double_t GetTLCal  () const {return GetTCal(0);}  
  virtual Double_t GetTRCal  () const {return GetTCal(1);}  
  virtual Double_t GetTLSlw  () const {return GetTSlw(0);}  
  virtual Double_t GetTRSlw  () const {return GetTSlw(1);}  
  virtual Double_t GetTLCalT0() const {return GetTCalT0(0);}
  virtual Double_t GetTRCalT0() const {return GetTCalT0(1);}
  virtual Double_t GetTLSlwT0() const {return GetTSlwT0(0);}
  virtual Double_t GetTRSlwT0() const {return GetTSlwT0(1);}
  
  virtual Int_t    GetTLRawTRef  () const {return GetTRawTRef(0);}  
  virtual Int_t    GetTRRawTRef  () const {return GetTRawTRef(1);}  
  virtual Double_t GetTLCalTRef  () const {return GetTCalTRef(0);}  
  virtual Double_t GetTRCalTRef  () const {return GetTCalTRef(1);}  
  virtual Int_t    GetTLRawTSub  () const {return GetTRawTSub(0);}  
  virtual Int_t    GetTRRawTSub  () const {return GetTRawTSub(1);}  
  virtual Double_t GetTLCalTSub  () const {return GetTCalTSub(0);}  
  virtual Double_t GetTRCalTSub  () const {return GetTCalTSub(1);}  


  virtual Double_t GetDTRaw() const {return fDTRaw;}
  virtual Double_t GetDTCal() const {return fDTCal;}
  virtual Double_t GetDTSlw() const {return fDTSlw;}
  virtual Double_t GetTAveRaw() const {return fTAveRaw;}
  virtual Double_t GetTAveCal() const {return fTAveCal;}
  virtual Double_t GetTAveSlw() const {return fTAveSlw;}
  virtual Double_t GetTAveCalT0() const {return fTAveCalT0;}
  virtual Double_t GetTAveSlwT0() const {return fTAveSlwT0;}
  virtual Double_t GetTTOFGamma() const {return fTTOFGamma;}
  virtual Double_t GetTTOFNeutron() const {return fTTOFNeutron;}

  virtual Double_t GetPosCal() const {return fPosCal;}
  virtual Double_t GetPosSlw() const {return fPosSlw;}
  virtual const Double_t* GetPos() const {return fPos;}
  virtual Double_t GetPos(Int_t i) const {return fPos[i];}
  virtual Double_t GetFlightLength() const {return fFlightLength;}
  virtual Double_t GetFlightAngle() const {return fFlightAngle;}

private:
  Int_t    fLayer;  
  Int_t    fSubLayer;  
  Double_t fDetPos[3];
  Int_t    fHit;

  Int_t    fQRaw[2];
  Double_t fQPed[2];
  Double_t fQCal[2];

  Double_t fQAvePed;
  Double_t fQAveCal;
  Double_t fLogQPed;
  Double_t fLogQCal;
  Double_t fIvSqrtQPed[2];
  Double_t fIvSqrtQAvePed;

  Int_t    fTRaw[2];
  Double_t fTCal[2];
  Double_t fTSlw[2];
  
  Int_t    fTRawTRef[2];
  Double_t fTCalTRef[2];
  Int_t    fTRawTSub[2];
  Double_t fTCalTSub[2];

  Double_t fDTRaw;
  Double_t fDTCal;
  Double_t fDTSlw;
  Double_t fTAveRaw;
  Double_t fTAveCal;
  Double_t fTAveSlw;
  Double_t fTCalT0[2];
  Double_t fTSlwT0[2];
  Double_t fTAveCalT0;
  Double_t fTAveSlwT0;
  Double_t fTTOFGamma;
  Double_t fTTOFNeutron;

  Double_t fPosCal;
  Double_t fPosSlw;
  Double_t fPos[3];
  Double_t fFlightLength;
  Double_t fFlightAngle;

  DetDirectionDef fDetDirect;

  ClassDef(TArtHIMEPla,1);
};
#endif

