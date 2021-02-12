// NEBULA calibration class
// 

#ifndef _TARTCALIBNEBULA_H_
#define _TARTCALIBNEBULA_H_

#include <TNamed.h>
#include <vector>
#include <map>

#include "TArtReconstruction.hh"

#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"

#include <TClonesArray.h>
#include <iostream>
#include <cmath>

class TClonesArray;
class TArtStoreManager;
class TArtSAMURAIParameters;

class TArtNEBULAPla;
typedef TClonesArray TArtNEBULAPlaArray;
class TArtNEBULAPlaPara;
typedef TClonesArray TArtNEBULAPlaParaArray;
class TArtCalibSAMURAIT0;

class TString;

class TArtCalibNEBULAMult : public TObject
{
public:
  TArtCalibNEBULAMult();
  ~TArtCalibNEBULAMult();

  void ClearData();

  Int_t fMultiplicity_N;
  Int_t fMultiplicity_N1;
  Int_t fMultiplicity_N2;
  Int_t fMultiplicity_V1;
  Int_t fMultiplicity_V2;
  Int_t fMultiplicity_N11;
  Int_t fMultiplicity_N12;
  Int_t fMultiplicity_N21;
  Int_t fMultiplicity_N22;
  Double_t fNebula_tt;

  ClassDef(TArtCalibNEBULAMult,1)
};

class TArtCalibNEBULA : public TArtReconstruction
{
public:
  TArtCalibNEBULA();
  virtual ~TArtCalibNEBULA();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ReconstructData();
  void ClearData();

  Int_t GetNumNEBULAPla() const;
  TArtNEBULAPla* GetNEBULAPla(Int_t i) const;
  TArtNEBULAPla* FindNEBULAPla(Int_t id) const;//find by id
  TArtNEBULAPlaArray* GetNEBULAPlaArray() const;
  Int_t GetNumNEBULAPlaPara() const;
  TArtNEBULAPlaPara* GetNEBULAPlaPara(Int_t i) const;
  TArtNEBULAPlaPara* FindNEBULAPlaPara(Int_t id) const;//find by id
  TArtNEBULAPlaParaArray* GetNEBULAPlaParaArray() const;

  void  CalMultiplicity(); // calculate multiplicity

  void SetMultiplicity_N(Int_t val){fMultiplicity_N = val;}
  void SetMultiplicity_N1(Int_t val){fMultiplicity_N1 = val;}
  void SetMultiplicity_N2(Int_t val){fMultiplicity_N2 = val;}
  void SetMultiplicity_V1(Int_t val){fMultiplicity_V1 = val;}
  void SetMultiplicity_V2(Int_t val){fMultiplicity_V2 = val;}
  void SetMultiplicity_N11(Int_t val){fMultiplicity_N11 = val;}
  void SetMultiplicity_N12(Int_t val){fMultiplicity_N12 = val;}
  void SetMultiplicity_N21(Int_t val){fMultiplicity_N21 = val;}
  void SetMultiplicity_N22(Int_t val){fMultiplicity_N22 = val;}
  void SetTestTime(Double_t val){fNebula_tt = val;}
  
  Int_t GetMultiplicity_N(){  return fMultiplicity_N  ;}
  Int_t GetMultiplicity_N1(){ return fMultiplicity_N1 ;}
  Int_t GetMultiplicity_N2(){ return fMultiplicity_N2 ;}
  Int_t GetMultiplicity_V1(){ return fMultiplicity_V1 ;}
  Int_t GetMultiplicity_V2(){ return fMultiplicity_V2 ;}
  Int_t GetMultiplicity_N11(){return fMultiplicity_N11;}
  Int_t GetMultiplicity_N12(){return fMultiplicity_N12;}
  Int_t GetMultiplicity_N21(){return fMultiplicity_N21;}
  Int_t GetMultiplicity_N22(){return fMultiplicity_N22;}
  Double_t GetTestTime(){return fNebula_tt;}

  TArtCalibNEBULAMult* GetMultiplicities(){return &fMult;} 
 
private:
  //  TArtCalibSAMURAIT0* fCalibSAMURAIT0;
  TClonesArray* fT0Array;
  TArtNEBULAPlaArray* fNEBULAPlaArray;
  TArtNEBULAPlaParaArray* fNEBULAPlaParaArray;

  const TArtSAMURAIParameters* fSAMURAIParameters;

  TArtStoreManager* fStoreManager;

  TArtCalibNEBULAMult fMult;

  //for optimization
  std::map<int,int> fIDNPlaMap;
  std::map<int,int> fIDNPlaParaMap;

  ClassDef(TArtCalibNEBULA,1);

  Int_t fMultiplicity_N;
  Int_t fMultiplicity_N1;
  Int_t fMultiplicity_N2;
  Int_t fMultiplicity_V1;
  Int_t fMultiplicity_V2;
  Int_t fMultiplicity_N11;
  Int_t fMultiplicity_N12;
  Int_t fMultiplicity_N21;
  Int_t fMultiplicity_N22;
  Double_t fNebula_tt;
};

#endif
