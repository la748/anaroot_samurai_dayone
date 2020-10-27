// PDC hit calibration class
// 

#ifndef _TARTCALIBPDCHIT_H_
#define _TARTCALIBPDCHIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibPDCHit : public TArtCalibDCHit {

 public:

  TArtCalibPDCHit(const TString outdataname = "SAMURAIPDCHit");
  virtual ~TArtCalibPDCHit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibPDCHit,1);

};

#endif
