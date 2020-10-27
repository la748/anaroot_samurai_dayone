// S1DC1 hit calibration class
// 

#ifndef _TARTCALIBS1DC1HIT_H_
#define _TARTCALIBS1DC1HIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibS1DC1Hit : public TArtCalibDCHit {

 public:

  TArtCalibS1DC1Hit(const TString outdataname="SAMURAIS1DC1Hit");
  virtual ~TArtCalibS1DC1Hit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibS1DC1Hit,1);

};

#endif
