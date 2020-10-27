//Coincidence register calibration class
// 

#ifndef _TARTCALIBCOIN_H_
#define _TARTCALIBCOIN_H_

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtStoreManager;

class TArtCalibCoin : public TArtReconstruction {

 public:

  TArtCalibCoin(const TString outdataname = "EventInfo");
  virtual ~TArtCalibCoin();
  virtual void LoadData();
  virtual void LoadData(TArtRawSegmentObject *seg);
  virtual void ReconstructData(){LoadData();}
  virtual void ClearData();

  virtual bool IsChTrue(Int_t id);

  // Set following value to decode only one coincidence register
  virtual void SetFpl(Int_t val);
  virtual void SetAddress(Int_t val);

 protected:

  TArtStoreManager * sman;

  ClassDef(TArtCalibCoin,1);
  
 private:

  Int_t val;

  // fpl or address used for choose the coin which is docoded
  Int_t myfpl;
  Int_t myaddress; // usually efn given in each segment header

};

#endif
