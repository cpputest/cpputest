/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <rt_sys.h>
#include <cstring>
#include <ctime>
#include "Serial.h"
#include "lpc17xx_timer.h"

/* Standard IO device handles. */
#define STDIN   0x8001
#define STDOUT  0x8002
#define STDERR  0x8003


#pragma import(__use_no_semihosting_swi)

const char __stdin_name[] = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";


FILEHANDLE _sys_open(const char *name, int openmode)
{
    FILEHANDLE rc;
    /* Register standard Input Output devices. */
    if (strcmp(name, "STDIN") == 0) rc = STDIN;
    else if (strcmp(name, "STDOUT") == 0) rc = STDOUT;
    else if (strcmp(name, "STDERR") == 0) rc = STDERR;
    else rc = 1;
    return (rc);
}

int _sys_close(FILEHANDLE fh)
{
  return 0;
}

int _sys_write(FILEHANDLE fh, const unsigned char *buf,
               unsigned len, int mode)
{
    while (len) {
        SER_PutChar(*buf);
        buf++;
        len--;
    }
    return len;
}

int _sys_read(FILEHANDLE fh, unsigned char *buf,
              unsigned len, int mode)
{
  return -1; /* not supported */
}

void _ttywrch(int ch)
{
    SER_PutChar(ch);
}

int _sys_istty(FILEHANDLE fh)
{
  return 0; /* buffered output */
}

int _sys_seek(FILEHANDLE fh, long pos)
{
  return -1; /* not supported */
}

long _sys_flen(FILEHANDLE fh)
{
  return -1; /* not supported */
}

void _sys_exit(int return_code) {
    while(1);
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
    const TIM_TIMERCFG_Type config_us_timer = {TIM_PRESCALE_USVAL, {0,0,0}, 1000}; /* 1000 us for one timer tick */
    TIM_Init(LPC_TIM3, TIM_TIMER_MODE, (void *)&config_us_timer); /* TIMER3 for counting 1 ms */
    TIM_Cmd(LPC_TIM3, ENABLE);
}

clock_t clock(void)
{
    return LPC_TIM3->TC;
}

time_t time(time_t *timer)
{
    return 0;
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
