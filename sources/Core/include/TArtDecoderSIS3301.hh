/*
 * @file TArtDecoderSIS3301.h
 * @date  Created : 
 *  Last Modified : Jul 15, 2014 03:18:03 JST
 *--------------------------------------------------------
 *    Comment : 
 *
 *--------------------------------------------------------
 *    Copyright (C)2008-2013 by Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */
#ifndef TARTDECODERSIS3301_H
#define TARTDECODERSIS3301_H
#include <TArtDecoder.hh>
#include "segidlist.hh"

class TArtDecoderSIS3301  : public TArtDecoder {
private:
   TArtDecoderSIS3301();         // constructor with default id = kID for compatibility
   TArtDecoderSIS3301(const TArtDecoderSIS3301&rhs) {;}

public:
   static const int kID = SIS3301;
   static TArtDecoder* Instance();
   virtual ~TArtDecoderSIS3301();
   virtual int Decode(unsigned char* &buf, const unsigned int& size,
		      TArtRawSegmentObject *rawseg);

   static const unsigned int kHeaderMask        = 0xf0000000;
   static const unsigned int kHeaderMask2       = 0xff000000;
   static const unsigned int kADCEvtHeader      = 0xc0000000;
   static const unsigned int kADCTIMEHeader     = 0x8a000000;
   static const unsigned int kMaskGeometry      = 0x1fff0000;
   static const unsigned int kMaskChannel       = 0x000000ff;
   static const unsigned int kMaskPagesize      = 0x1f000000;
   static const unsigned int kMaskEventID       = 0x0000ff00;
   static const unsigned int kMaskClock         = 0x00ffffff;
   static const unsigned int kMaskData1st       = 0x0000ffff;
   static const unsigned int kMaskData2nd       = 0xffff0000;
   static const int kShiftGeometry     = 16;
   static const int kShiftChannel      =  0;
   static const int kShiftPagesize     = 24;
   static const int kShiftEventID      =  8;
   static const int kShiftClock        =  0;
   static const int kShiftData1st      =  0;
   static const int kShiftData2nd      = 16;

};
#endif // end of #ifdef TARTDECODERSIS3301_H
