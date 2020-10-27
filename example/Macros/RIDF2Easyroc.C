
void RIDF2Easyroc(){

  gSystem->Load("libanaroot.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open("test.ridf");
  TArtRawEventObject *rawevent = estore->GetRawEventObject();

  TFile *fout = new TFile("ridf.root","RECREATE");
  TNtuple *ntp = new TNtuple("ntp","ntp","type:ch:val");

  int neve = 0;
  while(estore->GetNextEvent() && neve < 10){
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      if(35 == seg->GetModule())
	for(int j=0;j<seg->GetNumData();j++){
	TArtRawDataObject *d = seg->GetData(j);
	int geo = d->GetGeo(); 
	int ch = d->GetCh();
	int val = d->GetVal(); 
	ntp->Fill(geo,ch,val); 

	/*
	 * @file TArtDecoderVmeEasyroc.cc
	 * geo:0 High gain ADC
	 * geo:1 Low gain ADC
	 * geo:2 Leading edge TDC
	 * geo:3 Trailing edge TDC
	 * geo:4 Scaler
	 */

      }
    }

    estore->ClearData();
    neve ++;
  }

  fout->Write();
  fout->Close();

}

