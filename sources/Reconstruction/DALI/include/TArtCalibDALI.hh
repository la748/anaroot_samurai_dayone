// DALI calibration class
// 

#ifndef _TARTCALIBDALI_H_
#define _TARTCALIBDALI_H_

#include <TNamed.h>
#include <TRandom.h>

#include "TArtReconstruction.hh"
#include "TArtCalibCoin.hh"

class TArtRawSegmentObject;
class TArtDALINaIPara;
class TArtDALIParameters;
class TArtDALINaI;
class TArtStoreManager;

class TArtCalibDALI : public TArtReconstruction {

 public:

  TArtCalibDALI(const TString outdataname="DALINaI");
  virtual ~TArtCalibDALI();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void LoadFile(char s[]); // function to load dali configuration
  void ClearData();
  void ReconstructData();

  // function for doppler correct
  void DopplerCorrect();
  void DopplerCorrect(Double_t beta);
  void DopplerCorrectVertex(Double_t beta,Double_t vertex=0.0);
  // ****************

  // function to access data container
  TClonesArray    * GetNaIArray(){return fNaIArray;}
  Int_t             GetNumNaI();
  TArtDALINaI     * GetNaI(Int_t i);
  TArtDALINaIPara * GetNaIPara(Int_t i);
  // looking for nai whose id is i. return NULL in the case of fail to find.
  TArtDALINaI     * FindNaI(Int_t i);

  Double_t fBeta;
  Double_t fTimeTrueCutLow;
  Double_t fTimeTrueCutHigh;
  Double_t fMultiplicityThreshold;

  Int_t GetMultWithoutT(){return fMultWithoutT;}
  Int_t GetMult(){return fMult;}
  Int_t GetTimeTrueMult(){return fTimeTrueMult;}
  Int_t GetMultThres(){return fMultThres;}
  Int_t GetTimeTrueMultThres(){return fTimeTrueMultThres;}

  Double_t GetVertex(){return fVertex;}
  Double_t GetBeta(){return fBeta;}

  Double_t SetPlTime(Double_t a){return fPlTOF=a;}
  Double_t SetVertex(Double_t a){return fVertex=a;}
  Double_t SetBeta(Double_t a){return fBeta=a;}

 private:

  //TArtCalibCoin      * fCalibCoin; // removed. usually defined in global reco class
  TClonesArray       * fNaIArray;
  // temporal buffer for pointer for nai parameter
  std::vector <TArtDALINaIPara*> fNaIParaArray;

  TArtDALIParameters * setup;
  TArtStoreManager * sman;

  Double_t fMultWithoutT;
  Double_t fMult;
  Double_t fTimeTrueMult;
  Double_t fMultThres;
  Double_t fTimeTrueMultThres;

  Double_t fPlTOF;
  Double_t fVertex;

  TRandom *r;

  ClassDef(TArtCalibDALI,1);

 };

#endif
