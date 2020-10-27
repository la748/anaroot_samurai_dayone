// FDC0 SimpleTrack reconstruction class
// 

#ifndef _TARTCALIBSIMPLEFDC0TRACK_H_
#define _TARTCALIBSIMPLEFDC0TRACK_H_

#include <TROOT.h>

#include "TArtCalibSimpleDCTrack.hh"

class TArtCalibSimpleFDC0Track : public TArtCalibSimpleDCTrack {

 public:

  TArtCalibSimpleFDC0Track(const TString indataname="SAMURAIFDC0Hit",
		     const TString outdataname="SAMURAIFDC0Track");
  virtual ~TArtCalibSimpleFDC0Track();

  ClassDef(TArtCalibSimpleFDC0Track,1);

};

#endif
