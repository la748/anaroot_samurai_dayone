// ESPRI Diamond calibration class
// 

#ifndef _TARTCALIBDIA_H_
#define _TARTCALIBDIA_H_

#include <TClonesArray.h>
#include <TNamed.h>
#include "TArtReconstruction.hh"

class TArtDia;
class TArtESPRIParameters;
class TArtStoreManager;

class TArtCalibDia : public TArtReconstruction {

 public:

  TArtCalibDia();
  virtual ~TArtCalibDia();

  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetDiaArray(){return fDiaArray;}
  Int_t             GetNumDia();
  TArtDia     * GetDia(Int_t i);
  // looking for nai whose id is i. return NULL in the case of fail to find.
  TArtDia     * FindDia(Int_t i);

 private:

  TClonesArray       * fDiaArray;
  // temporal buffer for pointer for nai parameter

  TArtESPRIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibDia,1);

 };

#endif
