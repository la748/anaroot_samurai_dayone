// S1DC2 hit calibration class
// 

#ifndef _TARTCALIBS1DC2HIT_H_
#define _TARTCALIBS1DC2HIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibS1DC2Hit : public TArtCalibDCHit {

 public:

  TArtCalibS1DC2Hit(const TString outdataname="SAMURAIS1DC2Hit");
  virtual ~TArtCalibS1DC2Hit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibS1DC2Hit,1);

};

#endif
