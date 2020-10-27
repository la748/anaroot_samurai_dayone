/*
 * @file TArtDecoderA3400.cc
 * @date  Created : 2008/11/26 18:59:56 JST<BR>
 *  Last Modified : 2008/11/27 00:19:14 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderA3400.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
TArtDecoderA3400::TArtDecoderA3400()
   : TArtDecoder(kID) {
}

TArtDecoderA3400::~TArtDecoderA3400()
{
}

TArtDecoder* TArtDecoderA3400::Instance()
{
   static TArtDecoderA3400 instance;
   return &instance;
}

int TArtDecoderA3400::Decode(unsigned char* &buf, const unsigned int& size,
			     TArtRawSegmentObject* rawseg)
{

   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, igeo, ich;

   
   TArtCore::Info("TArtDecoderA3400","*** size= %d",evtsize);

   igeo = 0;
   for (i=0; i<evtsize; i++) {
     TArtCore::Info("TArtDecoderA3400","decode %x",evtdata[i]);
     if((evtdata[i]&kHeaderMask) == kCounterMask){
       TArtCore::Info("TArtDecoderA3400", "Header %d\n", igeo);
       igeo ++;
       continue;
     }
     else if(((evtdata[i]&kHeaderMask) == kDataMask)){
       ich = (evtdata[i]&0x0003c000) >> 14;
       TArtCore::Info("TArtDecoderA3400", "Data %d %d\n", igeo, ich);
       TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,evtdata[i]&0x1fff);
       rawseg->PutData(rdata);
     }
   }
   return 0;
}
