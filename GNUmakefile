.EXPORT_ALL_VARIABLES:
buildbits 	?= $(shell getconf LONG_BIT)

SUBDIRS		:= CommonUtil MultiFunctionLogger SimpleTransmissionMiddleware SyncMemoryService 
#SUBDIRS		:= CommonUtil MultiFunctionLogger SimpleTransmissionMiddleware SyncMemoryService stm_example
TOPDIR		:= $(shell pwd)
LIBPATH_SUFFIX	:=

ifeq ($(buildbits),64)
LIBPATH_SUFFIX	:= 64
endif

LIBPATH		:= $(TOPDIR)/lib$(LIBPATH_SUFFIX)
USRLIBPATH	:= /usr/lib$(LIBPATH_SUFFIX)
#file exists check
#ifeq ($(wildcard  ./include/version.h),)
#$(shell cp ./version.h ./include/version.h)
#$(shell cat version.h.in | sed -s "s/@SVNVERSION@/`svnversion -n`/g" > ./include/version.h)
#svn propset svn:keywords "Date Rev" version.h.in
#endif


all:
	$(foreach dir,$(SUBDIRS), make -C $(dir)  || exit $?;)

clean:
	$(foreach dir,$(SUBDIRS), make -C $(dir) clean  || exit $?;)

dos2unix:
	@for f in `find . -type f \( -iname "*.cpp" -o -iname "*.h" -o -iname "*.tmp" -o -iname "*.cxx" -o -iname "*.hpp" \)`; do    sed -i 's/\r//' $$f ; done
#$(foreach dir,$(SUBDIRS), make -C $(dir) dos2unix  || exit $?;)

