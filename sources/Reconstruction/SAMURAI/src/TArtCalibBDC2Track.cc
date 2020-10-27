#include "TArtCalibDCTrack.hh" 
#include "TArtCalibBDC2Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibBDC2Track::TArtCalibBDC2Track(const TString indataname,
				       const TString outdataname) 
  : TArtCalibDCTrack(indataname, outdataname, "XXYYXXYY", 3)
{
  TArtCore::Info(__FILE__,"Creating the BDC2 detector track objects...");
  for(int i=0;i<8;i++) pitch[i] = 5; // mm 
}

//__________________________________________________________
TArtCalibBDC2Track::~TArtCalibBDC2Track()  {
}

