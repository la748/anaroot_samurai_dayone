
SUBSRC0 = $(wildcard src/*.cc)
SUBHDR0 = $(wildcard include/*.hh) 

if USE_MINOS
SUBSRC := $(SUBSRC0)
SUBHDR := $(SUBHDR0)
else
SUBSRC := $(shell l=($(SUBSRC0)); echo $${l[@]//*MINOS*})
SUBHDR := $(shell l=($(SUBHDR0)); echo $${l[@]//*MINOS*})
endif

SUBOBJ = $(SUBSRC:.cc=.o)

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../Core/include -I../../Core/include -I../../Nadeko/include $(patsubst %,-I%,$(wildcard ../../Reconstruction/*/include))
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -g -Wall  -I../Core/include -I../../Core/include -I../../Nadeko/include $(patsubst %,-I%,$(wildcard ../../Reconstruction/*/include))
if USE_MINOS
CXXFLAGS += -DUSE_MINOS
endif
LDFLAGS = $(shell root-config --glibs)

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanaloopencexample.la
libanaloopencexample_la_SOURCES = $(SUBSRC) analoopencexample_dict.cc
libanaloopencexample_la_DEPENDENCIES = $(SUBOBJ)
libanaloopencexample_la_LIBADD = $(SUBOBJ)
libanaloopencexample_la_LDFLAGS = $(LDFLAGS)

if USE_MINOS
ROOTCINTFLAGS = $(DEFAULT_INCLUDES) $(INCLUDES) -DUSE_MINOS
else
ROOTCINTFLAGS = $(DEFAULT_INCLUDES) $(INCLUDES)
endif

analoopencexample_dict.cc: $(SUBHDR) analoopencexample_linkdef.hh
	rootcint -f $@ -c $(ROOTCINTFLAGS) $^
	@if [ ! -d $(prefix)/lib ]; then \
	mkdir -p $(prefix)/lib; \
	fi
	@if [ -f analoopencexample_dict_rdict.pcm ]; then \
	cp analoopencexample_dict_rdict.pcm $(prefix)/lib; \
	fi

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
