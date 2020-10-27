// S1DC1 Track reconstruction class
// 

#ifndef _TARTCALIBS1DC1TRACK_H_
#define _TARTCALIBS1DC1TRACK_H_

#include <TROOT.h>

#include "TArtCalibDCTrack.hh"

class TArtCalibS1DC1Track : public TArtCalibDCTrack {

 public:

  TArtCalibS1DC1Track(const TString indataname="SAMURAIS1DC1Hit",
		     const TString outdataname="SAMURAIS1DC1Track");
  virtual ~TArtCalibS1DC1Track();

  ClassDef(TArtCalibS1DC1Track,1);

};

#endif
