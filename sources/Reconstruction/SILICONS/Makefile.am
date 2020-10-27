SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../../Core/include -I../../Reconstruction/BigRIPS/include  -I../../Reconstruction/SAMURAI/include 
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I../../Core/include -I../../Reconstruction/BigRIPS/include  -I../../Reconstruction/SAMURAI/include 
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanasilicons.la
libanasilicons_la_SOURCES = $(SUBSRC1) anasilicons_dict.cc
libanasilicons_la_DEPENDENCIES = $(SUBOBJ)
libanasilicons_la_LIBADD = $(SUBOBJ)
libanasilicons_la_LDFLAGS = $(LDFLAGS)

anasilicons_dict.cc: $(SUBHDR) anasilicons_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
