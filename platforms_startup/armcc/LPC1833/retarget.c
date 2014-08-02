/*
 * @brief	IO redirection support
 *
 * This file adds re-direction support to the library for various
 * projects. It can be configured in one of 3 ways - no redirection,
 * redirection via a UART, or redirection via semihosting. If DEBUG
 * is not defined, all printf statements will do nothing with the
 * output being throw away. If DEBUG is defined, then the choice of
 * output is selected by the DEBUG_SEMIHOSTING define. If the
 * DEBUG_SEMIHOSTING is not defined, then output is redirected via
 * the UART. If DEBUG_SEMIHOSTING is defined, then output will be
 * attempted to be redirected via semihosting. If the UART method
 * is used, then the Board_UARTPutChar and Board_UARTGetChar
 * functions must be defined to be used by this driver and the UART
 * must already be initialized to the correct settings.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */


/* Keil (Realview) support */
#include <stdio.h>
#include <rt_sys.h>
#include <string.h>
#include <time.h>

#include "cmsis_18xx.h"


#pragma import(__use_no_semihosting_swi)

/* Standard IO device handles. */
const FILEHANDLE STDIN  = 0x8001;
const FILEHANDLE STDOUT = 0x8002;
const FILEHANDLE STDERR = 0x8003;

const char __stdin_name[] = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";

struct __FILE { int handle; /* Add whatever you need here */ };



/* number of system timer ticks */
static time_t timer_ticks = 0;


FILEHANDLE _sys_open(const char *name, int openmode)
{
    FILEHANDLE rc;
    /* Register standard Input Output devices. */
    if (strcmp(name, "STDIN") == 0) rc = STDIN;
    else if (strcmp(name, "STDOUT") == 0) rc = STDOUT;
    else if (strcmp(name, "STDERR") == 0) rc = STDERR;
    else rc = -1;
    return (rc);
}


int _sys_close(FILEHANDLE fh)
{
    return fh > 0x8000 ? 0 : 0;
}


/* Write to SWO */
void _ttywrch(int ch)
{
    ITM_SendChar(ch);
}


int _sys_istty(FILEHANDLE fh)
{
    return fh > 0x8000 ? 1 : 0;
}


int _sys_seek(FILEHANDLE fh, long pos)
{
    return fh > 0x8000 ? -1 : 0;
}


long _sys_flen(FILEHANDLE fh)
{
    return fh > 0x8000 ? 0 : -1;
}

/*--------------------------- _sys_tmpnam ------------------------------------*/
int _sys_tmpnam (char *name, int sig, unsigned maxlen)
{
  return (1);
}


int _sys_write(FILEHANDLE fh, const unsigned char *buf,
               unsigned len, int mode)
{
    int rc;
    if (fh == STDOUT) {
        while (len) {
            _ttywrch(*buf);
            buf++;
            len--;
            rc = 0;
        }
    } else if (fh > 0x8000) rc = -1;
    else rc = -1;
    return rc;
}

int _sys_read(FILEHANDLE fh, unsigned char *buf,
              unsigned len, int mode)
{
    return -1;
}


void _sys_exit(int return_code)
{
label:
	__WFI();
	goto label;	/* endless loop */
}


char *_sys_command_string(char *cmd, int len)
{
    const char * const arg[] = {
            "tst.axf",
            "-v",
            "-gSimpleStringBuffer",
            "-ojunit"};
    int i, j;
    char *rc = cmd;
    if (len) {
    /* Specify commandline arguments here as needed */
        len = sizeof(arg) / sizeof(char *);
        for (i = 0; i < len; i++) {
            j = strlen(arg[i])+1;
            if (cmd - rc + j > len) {
                rc = NULL;
                break;
            }
            memcpy(cmd, arg[i], j);
            cmd += j;
        }
        if (rc) rc = cmd;
    } else rc = NULL;
    return rc;
}


void _clock_init(void)
{
    SysTick_Config(CGU_IRC_FREQ / 1000); /* 1 ms system ticks interrupt period */
}


clock_t clock(void)
{
    return timer_ticks;
}


time_t time(time_t *timer)
{
    time_t rc = timer_ticks / CLOCKS_PER_SEC;
    if (timer) *timer = rc;
    return rc;
}


/* IRQ handler for Sys Tick timer */
__irq void SysTick_Handler(void)
{
    timer_ticks++;
}


/*
 * Замена встроенной в Keil MDK-ARM функции, которая создаёт связанный список
 * адресов деструкторов глобальных или статических объектов в "куче" при
 * инициализации окружения.
 * Так как не планируется завершение программы, то ни один такой деструктор не
 * должен вызываться, и можно подменить стандартную версию __aeabi_atexit на
 * другую, которая не будет создавать такой список в "куче".
 * Идея взята отсюда
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka3951.html
 * Пример реализации взят отсюда
 * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0041d/IHI0041D_cppabi.pdf
 */
int __aeabi_atexit(void* object, void (*destroyer)(void*), void* dso_handle)
{
    return 1;  // 0: failed; non-0: OK
//    return __cxa_atexit(destroyer, object, dso_handle); /* стандартная версия из Keil MDK-ARM */
}
