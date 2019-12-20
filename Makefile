CC              ?= gcc
CXX             ?= g++
LD              ?= ld
AR              ?= ar

SO_EXT          ?= so
LIB_EXT         ?= a
LIB_PREFIX      ?= lib

buildbits       ?= $(shell getconf LONG_BIT)

INCLUDES        :=      -I$(TOPDIR)/../include \

LIBPATH_SUFFIX  :=
LDEMULATION     := elf_i386

ifeq ($(buildbits),64)
LIBPATH_SUFFIX  := 64
LDEMULATION     := elf_x86_64
endif

LIBS            := -L$(TOPDIR)/../Release$(LIBPATH_SUFFIX)
LIBPATH         := $(TOPDIR)/../Release$(LIBPATH_SUFFIX)
USRLIBPATH      := /usr/lib$(LIBPATH_SUFFIX)

SHARED_LIBS_ONLY := 1

DEFINES         := -DLINUX
OPTIONS         := -O2

PARTIAL_SUFFIX  := _partial
LIB_SUFFIX      :=
OBJ_SUFFIX      :=

#BUILD_SET DEBUG, RELEASE
ifeq ($(DEBUG),1)
OPTIONS         := -g -O0  -W -Wall -Wno-missing-field-initializers -Wno-unused-parameter
PARTIAL_SUFFIX  := d_partial
LIB_SUFFIX      := d
OBJ_SUFFIX      := d
endif

CFLAGS          := -m$(buildbits) -fPIC $(OPTIONS)
CXXFLAGS        := -m$(buildbits) -fPIC $(OPTIONS)
SOFLAGS         := -m$(buildbits)
LDFLAGS         := -m $(LDEMULATION)
ARFLAGS         := rsc

OUTDIR          := objs$(LIBPATH_SUFFIX)
SRCDIR          := src



DIRECTORIES     := $(foreach dir, $(SUBDIRS), $(OUTDIR)/$(dir).dir)
IGNORE_FILES    := $(foreach file, $(IGNORE_FILES), $(SRCDIR)/$(file))
SRCS            := $(foreach dir, $(SUBDIRS), $(wildcard $(SRCDIR)/$(dir)/*.cpp))
SRCS            := $(filter-out $(IGNORE_FILES), $(SRCS))
OBJS            := $(SRCS:$(SRCDIR)/%.cpp=$(OUTDIR)/%$(OBJ_SUFFIX).o)

PARTIAL_OBJS    := $(foreach dir, $(SUBDIRS), $(dir)/$(OUTDIR)/$(notdir $(dir) )$(PARTIAL_SUFFIX).o)

PARTIAL_OBJ     := $(notdir $(shell pwd) )$(PARTIAL_SUFFIX).o
LIB_NAME        := $(LIB_PREFIX)$(TARGET_NAME)$(LIB_SUFFIX)

