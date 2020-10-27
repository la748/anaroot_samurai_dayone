// FDC2 Track reconstruction class
// 

#ifndef _TARTCALIBSIMPLEFDC2TRACK_H_
#define _TARTCALIBSIMPLEFDC2TRACK_H_

#include <TROOT.h>

#include "TArtCalibSimpleDCTrack.hh"

class TArtCalibSimpleFDC2Track : public TArtCalibSimpleDCTrack {

 public:

  TArtCalibSimpleFDC2Track(const TString indataname = "SAMURAIFDC2Hit",
		     const TString outdataname = "SAMURAIFDC2Track");
  virtual ~TArtCalibSimpleFDC2Track();

  ClassDef(TArtCalibSimpleFDC2Track,1);

};

#endif
