// PDC Track reconstruction class
// 

#ifndef _TARTCALIBPDCTRACK_H_
#define _TARTCALIBPDCTRACK_H_

#include <TROOT.h>

#include "TArtReconstruction.hh"
#include "TArtCalibDCTrack.hh" 
#include "TArtDCTrack.hh"
#include "TArtDCHit.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"


class TArtCalibPDCTrack : public TArtReconstruction {

 public:

  TArtCalibPDCTrack(const TString indata = "SAMURAIPDCHit",
		     const TString outdata = "SAMURAIPDCTrack");
  virtual ~TArtCalibPDCTrack();

  virtual void ClearData();
  virtual void ReconstructData();
  virtual void SetHitsBuffer();

  enum layer_type { x, y, u, v};

private:

  UShort_t nlayer ;
  UShort_t nlayer_x ;
  UShort_t nlayer_u ;
  UShort_t nlayer_v ;
  UShort_t nlayer_y ;

  std::vector<TArtDCHit*> *hits_buffer   ;
  std::vector<TArtDCHit*> *hits_buffer_x ;
  std::vector<TArtDCHit*> *hits_buffer_u ;
  std::vector<TArtDCHit*> *hits_buffer_v ;
  std::vector<TArtDCHit*> *hits_buffer_y ;

  std::vector<layer_type> layerdirs ;

  std::vector<UShort_t> layer_map ;

  TClonesArray *fDCTrackArray;
  TArtStoreManager*sman;

  TString indataname;
  TString outdataname;

  ClassDef(TArtCalibPDCTrack,1);

};

#endif
