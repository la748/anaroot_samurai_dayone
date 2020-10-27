/*
 * @file TArtDecoderA3400.h
 * @date  Created : 2009/03/26 12:41:31 JST<BR>
 *  Last Modified : 2009/03/26 12:41:41 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERA3400_H
#define TARTDECODERA3400_H
#include "TArtDecoder.hh"
class TArtDecoderA3400  : public TArtDecoder {
private:
   TArtDecoderA3400();
   TArtDecoderA3400(const TArtDecoderA3400&rhs) {;}

   static const unsigned int kHeaderMask        = 0xE0000000;
   static const unsigned int kCounterMask       = 0x60000000;
   static const unsigned int kDataMask          = 0x40000000;

public:
   static const int kID = 55;
   static TArtDecoder* Instance();
   ~TArtDecoderA3400();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg) ;
};
#endif // end of #ifdef TARTDECODERA3400_H
