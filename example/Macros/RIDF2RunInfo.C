
void RIDF2RunInfo(){

  gSystem->Load("libanaroot.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open("run0327.ridf.gz");
  TArtRawEventObject *rawevent = estore->GetRawEventObject();
  TArtRunInfo *rinfo = estore->GetRunInfo();
  cout << rinfo->GetHeader()->Data() << endl;

}

