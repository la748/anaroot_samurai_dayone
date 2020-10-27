make maintainer-clean
 find . -iname "*.o"   -exec rm -v {} \;
 find . -iname "*.so*" -exec rm -v {} \;
 find . -iname "*.la*" -exec rm -v {} \;
 find . -iname "*.lo"  -exec rm -v {} \;