// BDC1 Track reconstruction class
// 

#ifndef _TARTCALIBSIMPLEBDC1TRACK_H_
#define _TARTCALIBSIMPLEBDC1TRACK_H_

#include <TROOT.h>

#include "TArtCalibSimpleDCTrack.hh"

class TArtCalibSimpleBDC1Track : public TArtCalibSimpleDCTrack {

 public:

  TArtCalibSimpleBDC1Track(const TString indataname="SAMURAIBDC1Hit",
		     const TString outdataname="SAMURAIBDC1Track");
  virtual ~TArtCalibSimpleBDC1Track();

  ClassDef(TArtCalibSimpleBDC1Track,1);

};

#endif
