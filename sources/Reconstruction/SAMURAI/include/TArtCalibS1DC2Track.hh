// S1DC2 Track reconstruction class
// 

#ifndef _TARTCALIBS1DC2TRACK_H_
#define _TARTCALIBS1DC2TRACK_H_

#include <TROOT.h>

#include "TArtCalibDCTrack.hh"

class TArtCalibS1DC2Track : public TArtCalibDCTrack {

 public:

  TArtCalibS1DC2Track(const TString indataname="SAMURAIS1DC2Hit",
		     const TString outdataname="SAMURAIS1DC2Track");
  virtual ~TArtCalibS1DC2Track();

  ClassDef(TArtCalibS1DC2Track,1);

};

#endif
