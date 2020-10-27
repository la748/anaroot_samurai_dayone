/*
 * @file TArtDecoderMHINP.h
 * @date  Created : 2009/03/26 12:41:31 JST<BR>
 *  Last Modified : 2009/03/26 12:41:41 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERHINP_H
#define TARTDECODERHINP_H
#include "TArtDecoder.hh"
class TArtDecoderHINP  : public TArtDecoder {
private:
   TArtDecoderHINP();
   TArtDecoderHINP(const TArtDecoderHINP&rhs) {;}
   //TArtDecoderHINP& operator=(const TArtDecoderHINP&rhs) {;}
public:
   static const int kID = 56;
   static TArtDecoder* Instance();
   ~TArtDecoderHINP();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERHINP_H
