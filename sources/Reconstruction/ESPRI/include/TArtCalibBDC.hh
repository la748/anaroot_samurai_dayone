// BDC calibration class
// 

#ifndef _TARTCALIBBDC_H_
#define _TARTCALIBBDC_H_

#include <TClonesArray.h>
#include <TNamed.h>
#include "TArtReconstruction.hh"

class TArtBDC;
class TArtESPRIParameters;
class TArtUserParameters;
class TArtStoreManager;

class TArtCalibBDC : public TArtReconstruction {

 public:

  TArtCalibBDC();
  virtual ~TArtCalibBDC();

  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetBDCArray(){return fBDCArray;}
  Int_t             GetNumBDC();
  TArtBDC         * GetBDC(Int_t i);

private:

  TClonesArray       * fBDCArray;
  // temporal buffer for pointer for plastic parameter
  //std::vector <TArtBDCPara*> fBDCParaArray;

  TArtESPRIParameters * setup;
  TArtUserParameters * setup2;
  TArtStoreManager * sman;

  ClassDef(TArtCalibBDC,1);

};

#endif
