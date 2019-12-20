#DIRECTORIES	:= $(foreach dir, $(SUBDIRS), $(OUTDIR)/$(dir).dir)
PARENT_DIR	:= $(PARENT_DIR)/$(notdir $(shell pwd))
DIRECTORIES	:= $(foreach dir, $(SUBDIRS), $(OUTDIR)/$(dir).dir)
#IGNORE_FILES	:= $(foreach file, $(IGNORE_FILES), $(SRCDIR)/$(file))
#SRCS		:= $(foreach dir, $(SUBDIRS), $(wildcard $(SRCDIR)/$(dir)/*.cpp))
SRCS		:= $(wildcard $(shell pwd)/*.cpp)
#SRCS		:= $(filter-out $(IGNORE_FILES), $(SRCS))
OBJS		:= $(SRCS:$(SRCDIR)/%.cpp=$(OUTDIR)/%$(OBJ_SUFFIX).o)

PARTIAL_OBJS	:= $(foreach dir, $(SUBDIRS), $(dir)/$(OUTDIR)/$(notdir $(dir) )$(PARTIAL_SUFFIX).o) 

PARTIAL_OBJ	:= $(notdir $(shell pwd) )$(PARTIAL_SUFFIX).o
PARTIAL_SO	:= $(notdir $(shell pwd) )$(PARTIAL_SUFFIX).so
PARTIAL_A	:= $(notdir $(shell pwd) )$(PARTIAL_SUFFIX).a
LIB_NAME	:= $(LIB_PREFIX)$(TARGET_NAME)$(LIB_SUFFIX)
