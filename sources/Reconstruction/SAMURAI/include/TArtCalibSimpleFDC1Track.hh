// FDC1 Track reconstruction class
// 

#ifndef _TARTCALIBSIMPLEFDC1TRACK_H_
#define _TARTCALIBSIMPLEFDC1TRACK_H_

#include <TROOT.h>

#include "TArtCalibSimpleDCTrack.hh"

class TArtCalibSimpleFDC1Track : public TArtCalibSimpleDCTrack {

 public:

  TArtCalibSimpleFDC1Track(const TString indataname = "SAMURAIFDC1Hit",
		     const TString outdataname = "SAMURAIFDC1Track");
  virtual ~TArtCalibSimpleFDC1Track();

  ClassDef(TArtCalibSimpleFDC1Track,1);

};

#endif
