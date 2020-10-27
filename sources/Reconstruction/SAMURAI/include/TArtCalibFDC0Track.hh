// FDC0 Track reconstruction class
// 

#ifndef _TARTCALIBFDC0TRACK_H_
#define _TARTCALIBFDC0TRACK_H_

#include <TROOT.h>

#include "TArtCalibDCTrack.hh"

class TArtCalibFDC0Track : public TArtCalibDCTrack {

 public:

  TArtCalibFDC0Track(const TString indataname="SAMURAIFDC0Hit",
		     const TString outdataname="SAMURAIFDC0Track");
  virtual ~TArtCalibFDC0Track();

  ClassDef(TArtCalibFDC0Track,1);

};

#endif
