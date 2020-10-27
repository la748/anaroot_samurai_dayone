// BDC2 Track reconstruction class
// 

#ifndef _TARTCALIBSIMPLEBDC2TRACK_H_
#define _TARTCALIBSIMPLEBDC2TRACK_H_

#include <TROOT.h>

#include "TArtCalibSimpleDCTrack.hh"

class TArtCalibSimpleBDC2Track : public TArtCalibSimpleDCTrack {

 public:

  TArtCalibSimpleBDC2Track(const TString indataname = "SAMURAIBDC2Hit",
		     const TString outdataname = "SAMURAIBDC2Track");
  virtual ~TArtCalibSimpleBDC2Track();

  ClassDef(TArtCalibSimpleBDC2Track,1);

};

#endif
