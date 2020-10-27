// BDC1 Track reconstruction class
// 

#ifndef _TARTCALIBBDC1TRACK_H_
#define _TARTCALIBBDC1TRACK_H_

#include <TROOT.h>

#include "TArtCalibDCTrack.hh"

class TArtCalibBDC1Track : public TArtCalibDCTrack {

 public:

  TArtCalibBDC1Track(const TString indataname="SAMURAIBDC1Hit",
		     const TString outdataname="SAMURAIBDC1Track");
  virtual ~TArtCalibBDC1Track();

  ClassDef(TArtCalibBDC1Track,1);

};

#endif
