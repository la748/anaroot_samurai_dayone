// ESPRI Plastic calibration class
// 

#ifndef _TARTCALIBPLAS_H_
#define _TARTCALIBPLAS_H_

#include <TClonesArray.h>
#include <TNamed.h>
#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtPlasPara;
class TArtPlas;
class TArtESPRIParameters;
class TArtStoreManager;

class TArtCalibPlas : public TArtReconstruction {

 public:

  TArtCalibPlas();
  virtual ~TArtCalibPlas();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetPlasArray(){return fPlasArray;}
  Int_t             GetNumPlas();
  TArtPlas     * GetPlas(Int_t i);
  TArtPlasPara * GetPlasPara(Int_t i);
  // looking for nai whose id is i. return NULL in the case of fail to find.
  TArtPlas     * FindPlas(Int_t i);

 private:

  TClonesArray       * fPlasArray;
  // temporal buffer for pointer for nai parameter
  std::vector <TArtPlasPara*> fPlasParaArray;

  TArtESPRIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibPlas,1);

 };

#endif
