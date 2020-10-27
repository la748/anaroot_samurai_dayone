// Multihit TDC hit calibration class
// 

#ifndef _TARTCALIBTDCHIT_H_
#define _TARTCALIBTDCHIT_H_

#include <TClonesArray.h>
#include <TNamed.h>
#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtTDCHitPara;
class TArtTDCHit;
class TArtESPRIParameters;
class TArtStoreManager;

class TArtCalibTDCHit : public TArtReconstruction {

 public:

  TArtCalibTDCHit();
  virtual ~TArtCalibTDCHit();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetTDCHitArray(){return fTDCHitArray;}
  Int_t             GetNumTDCHit();
  TArtTDCHit     * GetTDCHit(Int_t i);
  TArtTDCHitPara * GetTDCHitPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  TArtTDCHit     * FindTDCHit(Int_t i);
  // looking for ic whose name is n. return NULL in the case of fail to find.
  TArtTDCHit     * FindTDCHit(TString *n);
  TArtTDCHit     * FindTDCHit(char *n);

  // protected:
private:

  Int_t  detectorid;
  TClonesArray       * fTDCHitArray;
  // temporal buffer for pointer for plastic parameter
  std::vector <TArtTDCHitPara*> fTDCHitParaArray;

  TArtESPRIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibTDCHit,1);

};

#endif
