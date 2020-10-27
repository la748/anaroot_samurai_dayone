#include "TArtCalibDCTrack.hh" 
#include "TArtCalibFDC0Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibFDC0Track::TArtCalibFDC0Track(const TString indataname, 
				       const TString outdataname) 
  : TArtCalibDCTrack(indataname, outdataname, "XXYYXXYY", 3)
{
  TArtCore::Info(__FILE__,"Creating the FDC0 detector track objects...");
  for(int i=0;i<8;i++) pitch[i] = 5; // mm 
}

//__________________________________________________________
TArtCalibFDC0Track::~TArtCalibFDC0Track()  {
}

