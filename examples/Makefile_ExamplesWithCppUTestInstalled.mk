
CPPFLAGS += -I /usr/local/include
LD_LIBRARIES = -L/usr/local/lib -lCppUTest -lCppUTestExt

VPATH = ApplicationLib AllTests

APPLIB_OBJECTS = CircularBuffer.o EventDispatcher.o hello.o Printer.o
TEST_OBJECTS = AllTests.o CircularBufferTest.o EventDispatcherTest.o FEDemoTest.o HelloTest.o MockDocumentationTest.o PrinterTest.o

CPPFLAGS += -I ApplicationLib

TEST_TARGET = CppUTestExamples
APPLIB = applicationLib.a

$(TEST_TARGET): $(TEST_OBJECTS) applicationLib.a
	$(CXX) -o $@ $^ $(LD_LIBRARIES) $(LDFLAGS)
	./$(TEST_TARGET)

$(APPLIB): $(APPLIB_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f -rf *.o
	rm -f $(TEST_TARGET)
	rm -f $(APPLIB)

