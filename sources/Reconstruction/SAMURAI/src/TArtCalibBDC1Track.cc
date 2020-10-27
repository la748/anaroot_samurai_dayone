#include "TArtCalibDCTrack.hh" 
#include "TArtCalibBDC1Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibBDC1Track::TArtCalibBDC1Track(const TString indataname, 
				       const TString outdataname) 
  : TArtCalibDCTrack(indataname, outdataname, "XXYYXXYY", 3)
{
  TArtCore::Info(__FILE__,"Creating the BDC1 detector track objects...");
  for(int i=0;i<8;i++) pitch[i] = 5; // mm 
}

//__________________________________________________________
TArtCalibBDC1Track::~TArtCalibBDC1Track()  {
}

