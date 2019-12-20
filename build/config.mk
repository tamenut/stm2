CC		?= gcc
CXX		?= g++
LD		?= ld
AR		?= ar

SO_EXT		?= so
LIB_EXT		?= a
LIB_PREFIX	?= lib

buildbits       ?= $(shell getconf LONG_BIT)

INCLUDES	:= 	-I$(TOPDIR)/include \
			-I$(TOPDIR)/include/CommonUtil \
			-I$(TOPDIR)/include/Log \
			-I$(TOPDIR)/include/SM \
			-I$(TOPDIR)/include/STM
LIBPATH_SUFFIX	:=
LDEMULATION	:= elf_i386

ifeq ($(buildbits),64)
LIBPATH_SUFFIX	:= 64
LDEMULATION	:= elf_x86_64
endif

LIBS		:= -L$(TOPDIR)/Release$(LIBPATH_SUFFIX)
LIBPATH		:= $(TOPDIR)/Release$(LIBPATH_SUFFIX)
USRLIBPATH	:= /usr/lib$(LIBPATH_SUFFIX)

# Version number of the libraries
VERSIONED_SO	:= 0#1= included revision num

#Shared Libs Only : not make *.a
SHARED_LIBS_ONLY := 1

DEFINES		:= -DLINUX -D_LINUX_ -Dlinux
OPTIONS		:= -O2

PARTIAL_SUFFIX	:= _partial
LIB_SUFFIX	:= 
OBJ_SUFFIX	:= 

#BUILD_SET DEBUG, RELEASE
ifeq ($(debug),1)
OPTIONS		:= -g -O0  -W -Wall -Wno-missing-field-initializers -Wno-unused-parameter 
PARTIAL_SUFFIX	:= d_partial
LIB_SUFFIX	:= d
OBJ_SUFFIX	:= d
endif

ifeq ($(prof),1)
OPTIONS     := $(OPTIONS) -pg
endif

CFLAGS		:= -m$(buildbits) -fPIC $(OPTIONS)
CXXFLAGS	:= -m$(buildbits) -fPIC $(OPTIONS)
SOFLAGS		:= -m$(buildbits)
LDFLAGS		:= -m $(LDEMULATION)
ARFLAGS		:= rsc

OUTDIR		:= $(TOPDIR)/objs$(LIBPATH_SUFFIX)
SRCDIR		:= $(TOPDIR)










