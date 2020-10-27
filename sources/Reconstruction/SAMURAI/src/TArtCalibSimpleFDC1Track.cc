#include "TArtCalibSimpleDCTrack.hh" 
#include "TArtCalibSimpleFDC1Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibSimpleFDC1Track::TArtCalibSimpleFDC1Track(const TString indataname,
				       const TString outdataname) 
  : TArtCalibSimpleDCTrack(indataname, outdataname, "XXUUVVXXUUVVXX",3)
{
  TArtCore::Info(__FILE__,"Creating the FDC1 detector track objects...");
  for(int i=0;i<14;i++) pitch[i] = 10; // mm 
}

//__________________________________________________________
TArtCalibSimpleFDC1Track::~TArtCalibSimpleFDC1Track()  {
}
