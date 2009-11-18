#include "CppUTest/CommandLineTestRunner.h"

int CppUTestMain()
{
  int ac = 2;
  char * av[] = {"test.exe", "-v", "-g", "Output"};
  return CommandLineTestRunner::RunAllTests(ac, av);
}

extern "C"
{
  int uartInit();
  int uartEcho();
  void uartPut(char*);
  void initLeds();
  void Led_TurnOff(int);
  void Led_TurnOn(int);
  void flashLeds();
}

int main()
{
  uartInit();
  initLeds();

  uartPut("\r\nRun CppUTests Tests\r\n");
  while (1)
  {
    CppUTestMain();
    uartEcho();
    flashLeds();
  }
}
