TARGET	:= GNUmakefile.$(notdir $(shell pwd))
SUBDIRS	:= #DiscoveryModule

all:
	$(foreach dir,$(SUBDIRS), make -C $(dir)  || exit $?;)
	make -f $(TARGET) all

clean:
	$(foreach dir,$(SUBDIRS), make -C $(dir) clean || exit $?;)
	make -f $(TARGET) clean

debug:
	$(foreach dir,$(SUBDIRS), make -C $(dir) debug || exit $?;)
	make -f $(TARGET) debug 

dos2unix:
	$(foreach dir,$(SUBDIRS), make -C $(dir) dos2unix || exit $?;)
	make -f $(TARGET) dos2unix

