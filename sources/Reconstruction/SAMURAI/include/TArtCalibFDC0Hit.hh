// FDC0 hit calibration class
// 

#ifndef _TARTCALIBFDC0HIT_H_
#define _TARTCALIBFDC0HIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibFDC0Hit : public TArtCalibDCHit {

 public:

  TArtCalibFDC0Hit(const TString outdataname="SAMURAIFDC0Hit");
  virtual ~TArtCalibFDC0Hit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibFDC0Hit,1);

};

#endif
