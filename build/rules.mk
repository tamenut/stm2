SRCS            := $(foreach dir, $(SUBDIRS), $(wildcard $(SRCDIR)/$(dir)/*.cpp))

compile:
	$(foreach dir,$(SUBDIRS), make -C $(dir)  || exit $?;)

distclean:
	$(foreach dir,$(SUBDIRS), make -C $(dir) clean;)
	rm -rf $(OUTDIR)/$(TARGET) $(OUTDIR)/*.o 

distdos2unix:
	$(foreach dir,$(SUBDIRS), make -C $(dir) dos2unix;)

distdebug:
	$(foreach dir,$(SUBDIRS), make -C $(dir) debug;)

realclean :
	rm -rf $(OUTDIR)/$(TARGET).a $(OUTDIR)/$(TARGET).so.0.0 $(foreach dir,$(SUBDIRS), $(OUTDIR)/$(dir)/*.o )

realdos2unix : 
	$(foreach dir,$(SUBDIRS), $(foreach ext, h cpp, -dos2unix $(SRCDIR)/$(dir)/*.$(ext); ) )


%.dir : 
	@if [ ! -d $* ]; then \
		echo "Making directory $*"; \
		mkdir -p $* ; \
	fi;


##########################################
.SUFFIXES : .o .c .cpp
%.o:%.c
	$(CC) $(CFLAGS) -o $@ $(DEFINES) $(INCLUDES) -c $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -o $(OUTDIR)/$@ $(DEFINES) $(INCLUDES) -c $<

$(OUTDIR)/%$(OBJ_SUFFIX).o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(DEFINES) $(INCLUDES) -c $<
##########################################

#TARGET := $(LIB_PREFIX)$(TARGET)
all : $(TARGET)

$(TARGET) : $(OBJS)
	@echo "TARET -> $(TARGET)"
	@echo "$(LIBPATH)"
	$(RM) $(TARGET).a
	$(AR) $(ARFLAGS) $(OUTDIR)/$(TARGET).a $(OBJS)
	$(CXX) $(SOFLAGS) -shared -Wl,-soname,$(USRLIBPATH)/$@.so.0 -o $(USRLIBPATH)/$@.so.0.0 $(OBJS)
	cp $(USRLIBPATH)/$(TARGET).so.0.0 $(OUTDIR)
	@if [ ! -L $(USRLIBPATH)/$(TARGET).so ]; then \
		ln -s $(USRLIBPATH)/$(TARGET).so.0.0 $(USRLIBPATH)/$(TARGET).so; \
		ln -s $(USRLIBPATH)/$(TARGET).so.0.0 $(USRLIBPATH)/$(TARGET).so.0; \
	fi;

dep :
	$(CC) -M $(INCLUDEDIRS) $(SRCS) > .depend
link :
	cp $(OUTDIR)/$(TARGET).so.0.0 $(LIBPATH)
	@if [ ! -L $(LIBPATH)/$(TARGET).so ]; then \
		ln -s $(LIBPATH)/$(TARGET).so.0.0 $(LIBPATH)/$(TARGET).so; \
		ln -s $(LIBPATH)/$(TARGET).so.0.0 $(LIBPATH)/$(TARGET).so.0; \
	fi;
