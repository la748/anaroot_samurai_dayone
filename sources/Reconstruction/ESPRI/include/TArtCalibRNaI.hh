// ESPRI NaI calibration class
// 

#ifndef _TARTCALIBRNAI_H_
#define _TARTCALIBRNAI_H_

#include <TClonesArray.h>
#include <TNamed.h>
#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtRNaIPara;
class TArtRNaI;
class TArtESPRIParameters;
class TArtStoreManager;

class TArtCalibRNaI : public TArtReconstruction {

 public:

  TArtCalibRNaI();
  virtual ~TArtCalibRNaI();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetRNaIArray(){return fRNaIArray;}
  Int_t             GetNumRNaI();
  TArtRNaI     * GetRNaI(Int_t i);
  TArtRNaIPara * GetRNaIPara(Int_t i);
  // looking for nai whose id is i. return NULL in the case of fail to find.
  TArtRNaI     * FindRNaI(Int_t i);

 private:

  TClonesArray       * fRNaIArray;
  // temporal buffer for pointer for nai parameter
  std::vector <TArtRNaIPara*> fRNaIParaArray;

  TArtESPRIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibRNaI,1);

 };

#endif
