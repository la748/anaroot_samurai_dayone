#include "TArtCalibDCTrack.hh" 
#include "TArtCalibS1DC1Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibS1DC1Track::TArtCalibS1DC1Track(const TString indataname, 
					 const TString outdataname) 
  : TArtCalibDCTrack(indataname, outdataname, "XXYY", -1)
{
  TArtCore::Info(__FILE__,"Creating the S1-MWDC #1 detector track objects...");
  pitch[0] = 12.; // X
  pitch[0] = 12.; // X'
  pitch[0] = 10.; // Y
  pitch[0] = 10.; // Y'
}

//__________________________________________________________
TArtCalibS1DC1Track::~TArtCalibS1DC1Track()  {
}

