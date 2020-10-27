// RDC calibration class
// 

#ifndef _TARTCALIBRDC_H_
#define _TARTCALIBRDC_H_

#include <TClonesArray.h>
#include <TNamed.h>
#include "TArtReconstruction.hh"

class TArtRDC;
class TArtESPRIParameters;
class TArtUserParameters;
class TArtStoreManager;

class TArtCalibRDC : public TArtReconstruction {

 public:

  TArtCalibRDC();
  virtual ~TArtCalibRDC();

  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetRDCArray(){return fRDCArray;}
  Int_t             GetNumRDC();
  TArtRDC         * GetRDC(Int_t i);

private:

  TClonesArray       * fRDCArray;
  // temporal buffer for pointer for plastic parameter
  //std::vector <TArtRDCPara*> fRDCParaArray;

  TArtESPRIParameters * setup;
  TArtUserParameters * setup2;
  TArtStoreManager * sman;

  ClassDef(TArtCalibRDC,1);

};

#endif
