#include "TArtCalibDCTrack.hh" 
#include "TArtCalibFDC2Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibFDC2Track::TArtCalibFDC2Track(const TString indataname,
				       const TString outdataname) 
  : TArtCalibDCTrack(indataname, outdataname, "XXUUVVXXUUVVXX")
{
  TArtCore::Info(__FILE__,"Creating the FDC2 detector track objects...");
  for(int i=0;i<14;i++) pitch[i] = 20; // mm 
}

//__________________________________________________________
TArtCalibFDC2Track::~TArtCalibFDC2Track()  {
}
