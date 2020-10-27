// CATANA calibration class
// 

#ifndef _TARTCALIBCATANA_H_
#define _TARTCALIBCATANA_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtCATANACsIPara;
class TArtCATANAParameters;
class TArtCATANACsI;
class TArtStoreManager;

class TArtCalibCATANA : public TArtReconstruction {

 public:

  TArtCalibCATANA();
  virtual ~TArtCalibCATANA();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function for doppler correct
  //void DopplerCorrect();
  void DopplerCorrect(Double_t beta=0.6);

  // function to access data container
  TClonesArray    * GetCsIArray(){return fCsIArray;}
  Int_t             GetCsIEntries();
  TArtCATANACsI     * GetCsI(Int_t i);
  TArtCATANACsIPara * GetCsIPara(Int_t i);
  // looking for nai whose id is i. return NULL in the case of fail to find.
  TArtCATANACsI     * FindCsI(Int_t i);

 private:

  TClonesArray       * fCsIArray;
  // temporal buffer for pointer for nai parameter
  std::vector <TArtCATANACsIPara*> fCsIParaArray;

  TArtCATANAParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibCATANA,1);

 };

#endif
