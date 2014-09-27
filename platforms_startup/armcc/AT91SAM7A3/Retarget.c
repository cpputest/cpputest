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
#include "lib_AT91SAM7A3.h"

/* Standard IO device handles. */
#define STDIN   0x8001
#define STDOUT  0x8002
#define STDERR  0x8003


#pragma import(__use_no_semihosting_swi)

const char __stdin_name[] = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";

static int init_UART_done = 0;
static void UART_putc(unsigned char);
static void UART_init(void);


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
      UART_putc(*buf);
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
  UART_putc(ch);
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
    AT91F_PITInit(AT91C_BASE_PITC, 1000, 50); /* Period = 1000 Hz, CLK = 50 MHz */
}

clock_t clock(void)
{
    static clock_t rc = 0;
    rc += AT91F_PITGetPIVR(AT91C_BASE_PITC) >> 20;
    return rc;
}

time_t time(time_t *timer)
{
    return 0;
}


static  void  UART_putc(unsigned char ch)
{
    if (init_UART_done == 0) {
        UART_init();
        init_UART_done++;
    }
    while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) == 0) __nop();
    AT91C_BASE_DBGU->DBGU_THR = ch;
}

static  void  UART_init(void)
{
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_SYS;
    AT91C_BASE_PIOA->PIO_PDR = (1<<30) | (1UL<<31);
    AT91C_BASE_PIOA->PIO_ASR = (1<<30) | (1UL<<31);
    AT91C_BASE_DBGU->DBGU_IDR = ~0;
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS | AT91C_US_TXDIS | AT91C_US_RSTSTA;
    AT91C_BASE_DBGU->DBGU_BRGR = 27; /* 115200 if MCLK = 50 МГц */
    AT91C_BASE_DBGU->DBGU_MR = AT91C_US_PAR_NONE | AT91C_US_CHMODE_NORMAL;
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN;
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTSTA; /* Reset status IRQ */
}

/* Replace of default Keil MDK-ARM function to create linked list of
 * destructor addresses of global or statically created objects. List is
 * placed in heap at startup.
 * No exit() is planned to call (exit from main() never take place in
 * embedded applications). So, it is no need to that list and it is possible
 * to use less heap memory.
 * New version of __aeabi_atexit does not create that list. See also
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka3951.html
 * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0041d/IHI0041D_cppabi.pdf
 */
int __aeabi_atexit(void* object, void (*destroyer)(void*), void* dso_handle)
{
    return 1;  // 0: failed; non-0: OK
//    return __cxa_atexit(destroyer, object, dso_handle); /* Keil MDK-ARM version */
}
