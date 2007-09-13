TARGET = AllTests

DEPENDENCY_FILE = .dependencies

LOADLIBS =\
 -lApplicationLib\
 -lCppUTest\
 -lstdc++\
 -lpthread

LIBDIR = ../lib

LIBDIRS =\
 -L$(CPP_U_TEST)/lib\
 -L$(LIBDIR)

INCLUDE =\
 -I.\
 -I$(CPP_U_TEST)/include

OBJS = 

include $(CPP_U_TEST)/build/TestExeMakefile

$(TARGET): $(TARGET).o $(DOTO) $(LIBDIR)/libApplicationLib.a
	$(CXX) -o $(TARGET) $(LIBDIRS) $^ $(LOADLIBS)

	