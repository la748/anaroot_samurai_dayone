#include "TArtCalibSimpleDCTrack.hh" 
#include "TArtCalibSimpleBDC1Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibSimpleBDC1Track::TArtCalibSimpleBDC1Track(const TString indataname, 
				       const TString outdataname) 
  : TArtCalibSimpleDCTrack(indataname, outdataname, "XXYYXXYY", 3)
{
  TArtCore::Info(__FILE__,"Creating the BDC1 detector track objects...");
  for(int i=0;i<8;i++) pitch[i] = 5; // mm 
}

//__________________________________________________________
TArtCalibSimpleBDC1Track::~TArtCalibSimpleBDC1Track()  {
}

