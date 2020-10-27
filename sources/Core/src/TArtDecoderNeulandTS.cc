/*
 * @file TArtDecoderNeulandTS.cc
 * modified by T. Isobe
 */
#include "TArtDecoderNeulandTS.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
#include <iostream>
TArtDecoderNeulandTS::TArtDecoderNeulandTS()
   : TArtDecoder(kID) {
}

TArtDecoderNeulandTS::~TArtDecoderNeulandTS()
{
}

TArtDecoder* TArtDecoderNeulandTS::Instance()
{
   static TArtDecoderNeulandTS instance;
   return &instance;
}

int TArtDecoderNeulandTS::Decode(unsigned char* &buf, const unsigned int& size,
                               TArtRawSegmentObject* rawseg)
{

   // do not put timestamp data in the meantime, because timestamp 
   // information is in header

   unsigned char tsc[8];
   unsigned long long int ts, tc;
   unsigned int ts0, ts1;   
   memcpy((char *)&tsc, buf, sizeof(tsc));
   
   ts = 0;
   // big endian -> little endian
   for(int i=0;i<8;i++){
     tc = tsc[i];
     ts = ts | (tc << (56-i*8));
   }
   ts0 = (ts >> 36) & 0x0000ffff;
   ts1 = (ts >> 4) & 0xffffffff;

   //printf("0x%08llx\n", ts);
   //std::cout << "****** " << ts << std::endl;

   // first 16bit is in ts0 and last 32bit is in ts1
   // original timestamp can be driven from 
   // unsigned long long int ts = ts0;
   // TArtCore::Info("TArtDecoderNeulandTS","ts: %lld",(ts<<32)+ts1);
   //TArtCore::Info("TArtDecoderNeulandTS","ts: %lld",timestamp);   


   TArtRawDataObject *rdata0 = new TArtRawDataObject(0,0,ts0);
   rawseg->PutData(rdata0);
   TArtRawDataObject *rdata1 = new TArtRawDataObject(0,1,ts1);
   rawseg->PutData(rdata1);
   return 0;

}
