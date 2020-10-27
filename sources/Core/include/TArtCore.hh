/*
 *    TArtCore.h
 *    Created       : 2008/09/26 18:09:23 JST
 *    Last Modified : Sep 05, 2009 13:31:57 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA
 */
#ifndef TARTCORE_H
#define TARTCORE_H
#include <TObject.h>
#include <TString.h>
#include <stdarg.h>


class TArtCore  {

 public:
  TArtCore();
  ~TArtCore();
  static void Debug(const char* cname,const char* fmt, ...);
  static void Info(const char* cname,const char* fmt, ...);
  static void Error(const char* cname,const char* fmt, ...);
  static void Warning(const char* cname,const char* fmt, ...);

  static TList* WarnList();
protected:
  static TList *warns;

 private:

  class WarnObj : public TObject {
  public:
    WarnObj() : nprint(0) {}
    ~WarnObj(){}
    TString mes;
    Int_t nprint;
  };

  ClassDef(TArtCore,1);

};
#endif // end of #ifdef TARTCORE_H
