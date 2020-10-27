// BLD calibration class
// 

#ifndef _TARTCALIBBLD_H_
#define _TARTCALIBBLD_H_

#include <TClonesArray.h>
#include <TNamed.h>
#include "TArtReconstruction.hh"

class TArtBLD;
class TArtESPRIParameters;
class TArtUserParameters;
class TArtStoreManager;

class TArtCalibBLD : public TArtReconstruction {

 public:

  TArtCalibBLD();
  virtual ~TArtCalibBLD();

  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetBLDArray(){return fBLDArray;}
  Int_t             GetNumBLD();
  TArtBLD         * GetBLD(Int_t i);

private:

  TClonesArray       * fBLDArray;
  // temporal buffer for pointer for plastic parameter
  //std::vector <TArtBLDPara*> fBLDParaArray;

  TArtESPRIParameters * setup;
  TArtUserParameters * setup2;
  TArtStoreManager * sman;

  ClassDef(TArtCalibBLD,1);

};

#endif
