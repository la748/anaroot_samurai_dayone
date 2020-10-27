SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../../Core/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I../../Core/include
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanaespri.la
libanaespri_la_SOURCES = $(SUBSRC1) anaespri_dict.cc
libanaespri_la_DEPENDENCIES = $(SUBOBJ)
libanaespri_la_LIBADD = $(SUBOBJ)
libanaespri_la_LDFLAGS = $(LDFLAGS)

anaespri_dict.cc: $(SUBHDR) anaespri_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^
	@if [ ! -d $(prefix)/lib ]; then \
	mkdir -p $(prefix)/lib; \
	fi
	@if [ -f anaespri_dict_rdict.pcm ]; then \
	cp anaespri_dict_rdict.pcm $(prefix)/lib; \
	fi

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
