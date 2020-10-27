#include "TArtCalibSimpleDCTrack.hh" 
#include "TArtCalibSimpleFDC0Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibSimpleFDC0Track::TArtCalibSimpleFDC0Track(const TString indataname, 
				       const TString outdataname) 
  : TArtCalibSimpleDCTrack(indataname, outdataname, "XXYYXXYY", 3)
{
  TArtCore::Info(__FILE__,"Creating the FDC0 detector track objects...");
  for(int i=0;i<8;i++) pitch[i] = 5; // mm 
}

//__________________________________________________________
TArtCalibSimpleFDC0Track::~TArtCalibSimpleFDC0Track()  {
}
