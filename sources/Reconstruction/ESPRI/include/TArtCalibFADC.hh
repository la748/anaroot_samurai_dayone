// Flush ADC hit calibration class
// 

#ifndef _TARTCALIBFADC_H_
#define _TARTCALIBFADC_H_

#include <TClonesArray.h>
#include <TNamed.h>
#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtRawFADCDataObjec;
class TArtFADCPara;
class TArtFADC;
class TArtESPRIParameters;
class TArtStoreManager;

class TArtCalibFADC : public TArtReconstruction {

 public:

  TArtCalibFADC();
  virtual ~TArtCalibFADC();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  //void LoadData(TArtRawSegmentObject *,TArtRawFADCDataObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetFADCArray(){return fFADCArray;}
  Int_t             GetNumFADC();
  TArtFADC     * GetFADC(Int_t i);
  TArtFADCPara * GetFADCPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  TArtFADC     * FindFADC(Int_t i);
  // looking for ic whose name is n. return NULL in the case of fail to find.
  TArtFADC     * FindFADC(TString *n);
  TArtFADC     * FindFADC(char *n);

  // protected:
private:

  Int_t  detectorid;
  TClonesArray       * fFADCArray;
  // temporal buffer for pointer for plastic parameter
  std::vector <TArtFADCPara*> fFADCParaArray;

  TArtESPRIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibFADC,1);

};

#endif
