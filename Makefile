# This Makefile shows nicely how to compile and link applications
# # using the ROOT libraries on all supported platforms.
# #
# # Copyright (c) 2000 Rene Brun and Fons Rademakers
# #
# # Author: Fons Rademakers, 29/2/2000
#
ARCH          = linux

CXX           =
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
MAINPATH      = /home/yuxip/FMS/CellScan
INCLUDEPATH   = $(MAINPATH)/include
INCLUDE       = -I$(INCLUDEPATH)
SOURCEPATH    = $(MAINPATH)/src
LIBPATH       = $(MAINPATH)/lib
LOCALLIB      = -L$(LIBPATH)

ifeq ($(ARCH),linux)
# Linux with egcs, gcc 2.9x, gcc 3.x (>= RedHat 5.2)
CXX           = g++
#CXXFLAGS      = -O -Wno-deprecated -fPIC -m32 -fno-inline -Wno-write-strings -g
CXXFLAGS      = $(ROOTCFLAGS) $(INCLUDE) -fPIC -g
LD            = g++
LDFLAGS       =  $(CXXFLAGS)
SOFLAGS       = -shared 

endif

CXXFLAGS      = $(ROOTCFLAGS) $(INCLUDE) -fPIC -g
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)
LDFLAGS       = $(CXXFLAGS)

#------------------------------------------------------------------------------
RESULTSONAME	= MyCellMgr.o MyCell.o MyCellRunDep.o MyCellEvt.o
RESULTSO        = $(patsubst %,$(SOURCEPATH)/%,$(RESULTSONAME))
RESULTSSNAME	= MyCellMgr.cpp MyCell.cpp MyCellRunDep.cpp MyCellEvt.cpp
RESULTSS	= $(patsubst %,$(SOURCEPATH)/%,$(RESULTSSNAME))
RESULTSSONAME	= MyCellMgr.o MyCell.o MyCellRunDep.o MyCellEvt.o MyCellMgrDict.o
RESULTSSO       = $(patsubst %,$(SOURCEPATH)/%,$(RESULTSSONAME))
RESULTSHNAME	= MyCellMgr.h MyCell.h MyCellRunDep.h MyCellEvt.h MyCellMgrDict.h
RESULTSH        = $(patsubst %,$(INCLUDEPATH)/%,$(RESULTSHNAME))
DICTDEPSH       = MyCellMgr.h MyCell.h MyCellRunDep.h MyCellEvt.h MyCellMgrLinkDef.h
DICTDEPS        = $(patsubst %,$(INCLUDEPATH)/%,$(DICTDEPSH)) 
#------------------------------------------------------------------------------

all : FillAdc $(LIBPATH)/libMyCellMgr.so

FillAdc: $(SOURCEPATH)/FillAdc.o $(LIBPATH)/libMyCellMgr.so
	 $(CXX) $(CXXFLAGS) $(SOURCEPATH)/FillAdc.o $(LOCALLIB) -lMyCellMgr $(GLIBS) -o $@
	 @echo "$@ done"
	 @rm -f $(SOURCEPATH)/*.o

$(LIBPATH)/libMyCellMgr.so:$(RESULTSSO) $(RESULTSH) $(RESULTSS)
		           $(LD) $(SOFLAGS) $(LDFLAGS) $(RESULTSSO) $(GLIBS) $(LOCALLIB) -lFpdchan -o $@

$(SOURCEPATH)/MyCellMgrDict.cpp: $(DICTDEPS)
		   @echo "Generating dictionary $@..."
		   @$(ROOTSYS)/bin/rootcint -f $@ -c $(INCLUDE) $(DICTDEPSH)
		   @mv $(SOURCEPATH)/MyCellMgrDict.h $(INCLUDEPATH)
		   @echo "$@ done..."

clean:
		@rm -f $(SOURCEPATH)/*.o $(LIBPATH)/libMyCellMgr.so $(SOURCEPATH)/*Dict.* $(INCLUDEPATH)/MyCellMgrDict.h
