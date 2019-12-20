
# Compile Option

#TOPDIR := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
TOPDIR := ../

# --- 기본적으로 수정되는 내용들 ! -----------------------------------

OUTDIR	:= objs64
TARGET = SyncServiceExample 
#SRCS	:= $(foreach dir, $(SUBDIRS), $(wildcard *.cpp))
SRCS    := $(foreach dir, ./, $(wildcard *.cpp))
#SRCS	:= example.cpp
OBJS 	:= $(addprefix $(OUTDIR)/, $(addsuffix .o, $(basename $(SRCS))))
LIBS	:= -L/usr/lib64/. -lrt -lpthread -lcommon_util -lMultiFunctionLogger -lstm -lsms

#---------------------------------------------------------------------

# --- 크로스 컴파일과 관련된 내용은 여기를 수정할것 ! ----------------
CC = gcc
CXX = g++
AR = ar
LD = ld
#---------------------------------------------------------------------

DEF        	:= -DLINUX -D_LINUX_ -Dlinux
CFLAGS		+= $(INCLUDEDIRS) -fPIC -Wall -O2 -g $(DEF)
CPPFLAGS	+= $(INCLUDEDIRS) -fPIC -Wall -O2 -g $(DEF)
ARFLAGS		= rsc

INCLUDEDIRS += -I./ \
	-I$(TOPDIR)/include/CommonUtil \
	-I$(TOPDIR)/include/STM \
	-I$(TOPDIR)/include/SM \
	-I$(TOPDIR)/include

ifeq ($(DEBUG),1)
CFLAGS          := $(INCLUDEDIRS) -g -fPIC -W -Wall -Wno-missing-field-initializers -Wno-unused-parameter -O0 $(DEF)
CPPFLAGS        := $(INCLUDEDIRS) -g -fPIC -W -Wall -Wno-missing-field-initializers -Wno-unused-parameter -O0 $(DEF)
endif


#
# Compilation target for C files
#
$(OUTDIR)/%.o:%.c
	@echo "Compiling $< ..."
	@echo "INC -> $(INCLUDEDIRS)"
	$(CC) -c $(CFLAGS) $(INCLUDEDIRS) -o $@ $<

#
# Compilation target for C++ files
#
$(OUTDIR)/%.o:%.cpp
	@echo "C++ compiling $< ..."
	@echo "INC -> $(INCLUDEDIRS)"
	$(CXX) -c $(CPPFLAGS) -o $@ $<

all : $(TARGET)

$(TARGET) : $(OBJS)
	@echo "SRCS-> $(SRCS)"
	@echo "OBJS -> $(OBJS)"
	@echo "TARET -> $(TARGET)"
	@echo "$(LIBPATH)"
	$(CXX) -g -o $(TARGET) $(OBJS) $(LIBS)

clean :
	rm -rf $(OBJS) $(TARGET)

