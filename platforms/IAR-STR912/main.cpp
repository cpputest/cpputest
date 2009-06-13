#include "CppUTest/CommandLineTestRunner.h"

int CppUTestMain()
{
  int ac = 4;
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
  /*
  Led_TurnOff(1);
  Led_TurnOff(2);
  Led_TurnOff(3);
  Led_TurnOff(4);
  Led_TurnOff(5);
  Led_TurnOff(6);
  Led_TurnOff(7);
  Led_TurnOff(8);
  Led_TurnOn(4);
  */
  uartPut("\r\nRun CppUTests Tests\r\n");
  while (1)
  {
    CppUTestMain();
    uartEcho();
    flashLeds();
  }
}
