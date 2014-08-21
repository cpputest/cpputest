/*-----------------------------------------------------------------------------
 * Name:    Serial.h
 * Purpose: Serial I/O definitions
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>

extern void     SER_Init    (void);
extern int32_t  SER_GetChar (void);
extern int32_t  SER_PutChar (int32_t ch);

#endif /* __SERIAL_H */
