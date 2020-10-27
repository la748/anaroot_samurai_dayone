SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../../Core/include -I../../Reconstruction/BigRIPS/include  -I../../Reconstruction/SAMURAI/include 
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I../../Core/include -I../../Reconstruction/BigRIPS/include  -I../../Reconstruction/SAMURAI/include 
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanacatana.la
libanacatana_la_SOURCES = $(SUBSRC1) anacatana_dict.cc
libanacatana_la_DEPENDENCIES = $(SUBOBJ)
libanacatana_la_LIBADD = $(SUBOBJ)
libanacatana_la_LDFLAGS = $(LDFLAGS)

anacatana_dict.cc: $(SUBHDR) anacatana_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^
	@if [ ! -d $(prefix)/lib ]; then \
	mkdir -p $(prefix)/lib; \
	fi
	@if [ -f anacatana_dict_rdict.pcm ]; then \
	cp anacatana_dict_rdict.pcm $(prefix)/lib; \
	fi

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
