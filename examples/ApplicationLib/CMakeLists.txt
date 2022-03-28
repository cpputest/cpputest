add_library(ApplicationLib
    CircularBuffer.cpp
    EventDispatcher.cpp
    hello.c
    Printer.cpp
)
target_include_directories(ExampleTests
    PUBLIC
        .
)

