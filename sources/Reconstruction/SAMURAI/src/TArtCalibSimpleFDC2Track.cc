#include "TArtCalibSimpleDCTrack.hh" 
#include "TArtCalibSimpleFDC2Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
//TArtCalibSimpleFDC2Track::TArtCalibSimpleFDC2Track(const TString indataname,
//				       const TString outdataname) 
//  : TArtCalibSimpleDCTrack(indataname, outdataname, "XXUUVVXXUUVVXX")
    //-----------------------------------
TArtCalibSimpleFDC2Track::TArtCalibSimpleFDC2Track(const TString indataname,
				       const TString outdataname) 
  : TArtCalibSimpleDCTrack(indataname, outdataname, "XXVVUUXXVVUUXX",3)
{
  TArtCore::Info(__FILE__,"Creating the FDC2 detector track objects...");
  for(int i=0;i<14;i++) pitch[i] = 20; // mm 
}

//__________________________________________________________
TArtCalibSimpleFDC2Track::~TArtCalibSimpleFDC2Track()  {
}
