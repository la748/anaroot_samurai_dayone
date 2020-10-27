// SSD calibration class
// 

#ifndef _TARTCALIBSILICONS_H_
#define _TARTCALIBSILICONS_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtSILICONSPara;
class TArtSILICONSParameters;
class TArtSILICONS;
class TArtStoreManager;

class TArtCalibSILICONS : public TArtReconstruction {

 public:

  TArtCalibSILICONS(const TString outdataname="SILICONS");
  virtual ~TArtCalibSILICONS();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetSiArray(){return fSiArray;}
  Int_t             GetNumSi();
  TArtSILICONS          * GetSi(Int_t i);
  TArtSILICONSPara      * GetSiPara(Int_t i);
  // looking for si whose id is i. return NULL in the case of fail to find.
  TArtSILICONS          * FindSi(Int_t id);

 private:

  TClonesArray       * fSiArray;
  // temporal buffer for pointer for ssd parameter
  std::vector <TArtSILICONSPara*> fSiParaArray;

  TArtSILICONSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibSILICONS,1);

 };

#endif
