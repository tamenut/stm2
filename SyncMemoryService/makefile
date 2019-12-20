
# Compile Option

#TOPDIR := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
TOPDIR := ../

# --- 기본적으로 수정되는 내용들 ! -----------------------------------
BUILD_BIT	:= $(shell getconf LONG_BIT)
OUTDIR	:= objs$(BUILD_BIT)
TARGET = libsms
SRCS	:= $(foreach dir, ./, $(wildcard *.cpp))
#SRCS	:= $(foreach dir, $(SUBDIRS), $(wildcard *.cpp))
#SRCS	:= Configuration.cpp DataAckMsg.cpp DataReader.cpp DataReaderImpl.cpp DataWriter.cpp DataWriterImpl.cpp
OBJS 	:= $(addprefix $(OUTDIR)/, $(addsuffix .o, $(basename $(SRCS))))
LIBPATH	:= /usr/lib$(BUILD_BIT)/

#---------------------------------------------------------------------

# --- 크로스 컴파일과 관련된 내용은 여기를 수정할것 ! ----------------
CC = gcc
CXX = g++
AR = ar
LD = ld
#---------------------------------------------------------------------

DEF        	:= -DLINUX -D_LINUX_ -Dlinux
CFLAGS		+= $(INCLUDEDIRS) -fPIC -Wall -O2 $(DEF)
CPPFLAGS	+= $(INCLUDEDIRS) -fPIC -Wall -O2 $(DEF)
ARFLAGS		= rsc

INCLUDEDIRS += -I./ \
	-I$(TOPDIR)/include/CommonUtil \
	-I$(TOPDIR)/include/STM \
	-I$(TOPDIR)/include/SM \
	-I$(TOPDIR)/include/Log \
	-I$(TOPDIR)/include

ifeq ($(DEBUG),1)
CFLAGS		:= $(INCLUDEDIRS) -g -fPIC -W -Wall -Wno-missing-field-initializers -Wno-unused-parameter -O0 $(DEF)
CPPFLAGS	:= $(INCLUDEDIRS) -g -fPIC -W -Wall -Wno-missing-field-initializers -Wno-unused-parameter -O0 $(DEF)
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
#%.o:%.cpp
	@echo "C++ compiling $< ..."
	@echo "INC -> $(INCLUDEDIRS)"
	$(CXX) -c $(CPPFLAGS) -o $@ $<

all : $(TARGET)

$(TARGET) : $(OBJS)
	@echo "SRCS-> $(SRCS)"
	@echo "OBJS -> $(OBJS)"
	@echo "TARET -> $(TARGET)"
	@echo "$(LIBPATH)"
	$(RM) $(TARGET).a
	$(AR) $(ARFLAGS)  $(TARGET).a $(OBJS)
	$(CXX) $(SOFLAGS) -shared -Wl,-soname,$@.so.0 -o $@.so.0.0 $(OBJS)
	cp $(TARGET).so.0.0 $(LIBPATH)
	@if [ ! -L $(LIBPATH)/$(TARGET).so ]; then \
		ln -s $(TARGET).so.0.0 $(TARGET).so; \
		ln -s $(TARGET).so.0.0 $(TARGET).so.0; \
	fi;
		
dep :
	$(CC) -M $(INCLUDEDIRS) $(SRCS) > .depend
link :
	cp $(TARGET).so.0.0 $(LIBPATH)
	@if [ ! -L $(LIBPATH)/$(TARGET).so ]; then \
		ln -s $(LIBPATH)/$(TARGET).so.0.0 $(LIBPATH)/$(TARGET).so; \
		ln -s $(LIBPATH)/$(TARGET).so.0.0 $(LIBPATH)/$(TARGET).so.0; \
	fi;
clean :
	rm -rf $(OBJS) $(TARGET).a $(TARGET).so.* core

