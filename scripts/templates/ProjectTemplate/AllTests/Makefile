CPPUTEST_TARGET = AllTests

DEPENDENCY_FILE = .dependencies

LOADLIBS =\
 -lProject\
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

$(CPPUTEST_TARGET): $(CPPUTEST_TARGET).o $(DOTO) $(LIBDIR)/libProject.a
	$(CXX) -o $(CPPUTEST_TARGET) $(LIBDIRS) $^ $(LOADLIBS)

	