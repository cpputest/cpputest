//- ------------------------------------------------------------------
//-    Copyright (c) James W. Grenning -- All Rights Reserved
//-    For use by owners of Test-Driven Development for Embedded C,
//-    and attendees of Renaissance Software Consulting, Co. training
//-    classes.
//-
//-    Available at http://pragprog.com/titles/jgade/
//-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3
//-
//-    Authorized users may use this source code in your own
//-    projects, however the source code may not be used to
//-    create training material, courses, books, articles, and
//-    the like. We make no guarantees that this source code is
//-    fit for any purpose.
//-
//-    www.renaissancesoftware.net james@renaissancesoftware.net
//- ------------------------------------------------------------------

#include "CppUTest/CommandLineTestRunner.h"
#include <iostream>
#include <cstdint>

extern "C" void _clock_init(void);
extern std::uint32_t SystemCoreClock;
extern const std::uint32_t SystemCoreClock12;

int main(int ac, char** av)
{
    SystemCoreClock = SystemCoreClock12; /* if 12 MHz quartz is used; need to LPC_TIM3 measure */
    _clock_init(); /* secondary call for new SystemCoreClock freq; previous call worked with unknown value */

    std::cout << "Hello, World!\n";
    int result = RUN_ALL_TESTS(ac, av);

    return result;
}

