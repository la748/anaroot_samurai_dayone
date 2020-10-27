SUBDIRS = Core Reconstruction Nadeko AnaLoop

lib_LTLIBRARIES = libanaroot.la
libanaroot_la_SOURCES =
libanaroot_la_LIBADD = \
 AnaLoop/Core/libanaloop.la \
 AnaLoop/Example/libanaloopexample.la \
 AnaLoop/EncExample/libanaloopencexample.la \
 Core/libanacore.la \
 Nadeko/libananadeko.la \
 Reconstruction/BigRIPS/libanabrips.la \
 Reconstruction/DALI/libanadali.la \
 Reconstruction/WINDS/libanawinds.la \
 Reconstruction/ESPRI/libanaespri.la \
 Reconstruction/SAMURAI/libanasamurai.la \
 Reconstruction/CATANA/libanacatana.la

if USE_MINOS
libanaroot_la_LIBADD += Reconstruction/MINOS/libanaminos.la 
endif
