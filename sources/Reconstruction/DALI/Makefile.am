SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../../Core/include -I../../Reconstruction/BigRIPS/include  -I../../Reconstruction/SAMURAI/include 
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I../../Core/include -I../../Reconstruction/BigRIPS/include  -I../../Reconstruction/SAMURAI/include 
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanadali.la
libanadali_la_SOURCES = $(SUBSRC1) anadali_dict.cc
libanadali_la_DEPENDENCIES = $(SUBOBJ)
libanadali_la_LIBADD = $(SUBOBJ)
libanadali_la_LDFLAGS = $(LDFLAGS)

anadali_dict.cc: $(SUBHDR) anadali_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^
	@if [ ! -d $(prefix)/lib ]; then \
	mkdir -p $(prefix)/lib; \
	fi
	@if [ -f anadali_dict_rdict.pcm ]; then \
	cp anadali_dict_rdict.pcm $(prefix)/lib; \
	fi

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
