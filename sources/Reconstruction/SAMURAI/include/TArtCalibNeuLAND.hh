// NeuLAND calibration class
// 

#ifndef _TARTCALIBNEULAND_H_
#define _TARTCALIBNEULAND_H_

#include <TNamed.h>
#include <vector>
#include <map>

#include "TArtReconstruction.hh"

#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "segidlist.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtNeuLANDPla.hh"
#include "TArtNeuLANDPlaPara.hh"

#include <TClonesArray.h>
#include <iostream>
#include <cmath>

class TClonesArray;
class TArtStoreManager;
class TArtSAMURAIParameters;

class TArtNeuLANDPla;
typedef TClonesArray TArtNeuLANDPlaArray;
class TArtNeuLANDPlaPara;
typedef TClonesArray TArtNeuLANDPlaParaArray;
class TArtCalibSAMURAIT0;

class TString;

class TArtCalibNeuLAND : public TArtReconstruction
{
public:
  TArtCalibNeuLAND();
  virtual ~TArtCalibNeuLAND();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ReconstructData();
  void ClearData();

  Int_t GetNumNeuLANDPla() const;
  TArtNeuLANDPla* GetNeuLANDPla(Int_t i) const;
  TArtNeuLANDPla* FindNeuLANDPla(Int_t id) const;//find by id
  TArtNeuLANDPlaArray* GetNeuLANDPlaArray() const;
  TArtNeuLANDPla const *GetNeuLANDMasterStart() const;

  Int_t GetNumNeuLANDPlaPara() const;
  TArtNeuLANDPlaPara* GetNeuLANDPlaPara(Int_t i) const;
  TArtNeuLANDPlaPara* FindNeuLANDPlaPara(Int_t id) const;//find by id
  TArtNeuLANDPlaParaArray* GetNeuLANDPlaParaArray() const;

private:
  //  TArtCalibSAMURAIT0* fCalibSAMURAIT0;
  TClonesArray* fT0Array;
  TArtNeuLANDPlaArray* fNeuLANDPlaArray;
  TArtNeuLANDPla *fNeuLANDMasterStart;
  TArtNeuLANDPlaParaArray* fNeuLANDPlaParaArray;

  const TArtSAMURAIParameters* fSAMURAIParameters;

  TArtStoreManager* fStoreManager;

  //for optimization
  std::map<int,int> fIDNPlaMap;
  std::map<int,int> fIDNPlaParaMap;
  TList *tac16_array;
  Int_t GetTac16(Int_t sam, Int_t gtb, Int_t mod) const;
  Double_t QDC2Energy(Double_t channel);
  Double_t WalkCorrection(Double_t e);

  class Tac16 : public TObject {
  public:
    Tac16(){sam=-1;gtb=-1;mod=-1;tac=-1;}
    ~Tac16(){}
    Int_t sam;
    Int_t gtb;
    Int_t mod;
    Int_t tac;

    // overriding functions for sorting based on sam, gtb and mod
    Bool_t IsEqual(TObject *obj) const { 
      return sam == ((Tac16*)obj)->sam &&
	gtb == ((Tac16*)obj)->gtb &&
	mod == ((Tac16*)obj)->mod;} 
    Bool_t IsSortable() const { return kTRUE; } 
    Int_t Compare(const TObject *obj) const { 
      if (sam < ((Tac16*)obj)->sam) return -1; 
      else if (sam > ((Tac16*)obj)->sam) return 1; 

      if (gtb < ((Tac16*)obj)->gtb) return -1; 
      else if (gtb > ((Tac16*)obj)->gtb) return 1; 

      if (mod < ((Tac16*)obj)->mod) return -1; 
      else if (mod > ((Tac16*)obj)->mod) return 1; 

      return 0; 
    }
  };


  ClassDef(TArtCalibNeuLAND,1);
};

#endif
