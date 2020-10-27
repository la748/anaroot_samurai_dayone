/*
 * @file TArtDecoderV1190C.h
 * @date  Created : 2008/11/26 21:30:29 JST<BR>
 *  Last Modified : 2008/11/27 00:18:53 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERV1190C_H
#define TARTDECODERV1190C_H
#include "TArtDecoder.hh"
class TArtDecoderV1190C  : public TArtDecoder {
private:
   TArtDecoderV1190C();
   TArtDecoderV1190C(const TArtDecoderV1190C&rhs) {;}
   //TArtDecoderV1190C& operator=(const TArtDecoderV1190C&rhs) {;}

public:
   static const int kID = 26;
   static TArtDecoder* Instance();
   virtual ~TArtDecoderV1190C();
   virtual int Decode(unsigned char* &buf, const unsigned int& size,
		      TArtRawSegmentObject *rawseg);

   static const unsigned int kHeaderMask        = 0xf8000000;
   static const unsigned int kGlobalHeader      = 0x40000000;
   static const unsigned int kTDCHeader         = 0x08000000;
   static const unsigned int kTDCMeasurement    = 0x00000000;
   static const unsigned int kTDCTrailer        = 0x18000000;
   static const unsigned int kTDCError          = 0x20000000;
   static const unsigned int kGlobalTrailer     = 0x80000000;
   static const unsigned int kMaskGeometry      = 0x0000001f;
   static const unsigned int kMaskEventCounter  = 0x7ffffe0;
   static const unsigned int kMaskBunchID       = 0x00000fff;
   static const unsigned int kMaskEventID       = 0x00000fff;
   static const unsigned int kMaskChannel       = 0x03f80000;
   static const unsigned int kMaskMeasure       = 0x0007ffff;
   static const unsigned int kMaskEdgeType      = 0x04000000;
   static const int kShiftGeometry     = 0;
   static const int kShiftEventCounter = 5;
   static const int kShiftBunchID      = 0;
   static const int kShiftEventID      = 12;
   static const int kShiftChannel      = 19;
   static const int kShiftMeasure      = 0;
   static const int kShiftEdgeType     = 26;

};
#endif // end of #ifdef TARTDECODERV1190C_H
