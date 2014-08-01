/**************************************************************************//**
 * @file     core_cmInstr.h
 * @brief    CMSIS Cortex-M Core Instruction Access Header File
 * @version  V3.20
 * @date     05. March 2013
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2009 - 2013 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef __CORE_CMINSTR_H
#define __CORE_CMINSTR_H


#pragma push
#pragma diag_suppress 667


/* ##########################  Core Instruction Access  ######################### */
/** \defgroup CMSIS_Core_InstructionInterface CMSIS Core Instruction Interface
  Access to dedicated instructions
  @{
*/

/* ARM armcc specific functions */

#if (__ARMCC_VERSION < 400677)
  #error "Please use ARM Compiler Toolchain V4.0.677 or later!"
#endif


/** \brief  No Operation

    No Operation does nothing. This instruction can be used for code alignment purposes.
 */
#define __NOP                             __nop


/** \brief  Wait For Interrupt

    Wait For Interrupt is a hint instruction that suspends execution
    until one of a number of events occurs.
 */
#define __WFI                             __wfi


/** \brief  Wait For Event

    Wait For Event is a hint instruction that permits the processor to enter
    a low-power state until one of a number of events occurs.
 */
#define __WFE                             __wfe


/** \brief  Send Event

    Send Event is a hint instruction. It causes an event to be signaled to the CPU.
 */
#define __SEV                             __sev


/** \brief  Instruction Synchronization Barrier

    Instruction Synchronization Barrier flushes the pipeline in the processor,
    so that all instructions following the ISB are fetched from cache or
    memory, after the instruction has been completed.
 */
#define __ISB()                           __isb(0xF)


/** \brief  Data Synchronization Barrier

    This function acts as a special kind of Data Memory Barrier.
    It completes when all explicit memory accesses before this instruction complete.
 */
#define __DSB()                           __dsb(0xF)


/** \brief  Data Memory Barrier

    This function ensures the apparent order of the explicit memory operations before
    and after the instruction, without ensuring their completion.
 */
#define __DMB()                           __dmb(0xF)


/** \brief  Reverse byte order (32 bit)

    This function reverses the byte order in integer value.

    \param [in]    value  Value to reverse
    \return               Reversed value
 */
#define __REV                             __rev


/** \brief  Reverse byte order (16 bit)

    This function reverses the byte order in two unsigned short values.

    \param [in]    value  Value to reverse
    \return               Reversed value
 */
#ifndef __NO_EMBEDDED_ASM
__attribute__((section(".rev16_text"))) static inline __asm uint32_t __REV16(uint32_t value)
{
  rev16 r0, r0
  bx lr
}
#endif

/** \brief  Reverse byte order in signed short value

    This function reverses the byte order in a signed short value with sign extension to integer.

    \param [in]    value  Value to reverse
    \return               Reversed value
 */
#ifndef __NO_EMBEDDED_ASM
__attribute__((section(".revsh_text"))) static inline __asm int32_t __REVSH(int32_t value)
{
  revsh r0, r0
  bx lr
}
#endif


/** \brief  Rotate Right in unsigned value (32 bit)

    This function Rotate Right (immediate) provides the value of the contents of a register rotated by a variable number of bits.

    \param [in]    value  Value to rotate
    \param [in]    value  Number of Bits to rotate
    \return               Rotated value
 */
#define __ROR                             __ror


/** \brief  Breakpoint

    This function causes the processor to enter Debug state.
    Debug tools can use this to investigate system state when the instruction at a particular address is reached.

    \param [in]    value  is ignored by the processor.
                   If required, a debugger can use it to store additional information about the breakpoint.
 */
#define __BKPT(value)                       __breakpoint(value)


#if       (__CORTEX_M >= 0x03)

/** \brief  Reverse bit order of value

    This function reverses the bit order of the given value.

    \param [in]    value  Value to reverse
    \return               Reversed value
 */
#define __RBIT                            __rbit


/** \brief  LDR Exclusive (8 bit)

    This function performs a exclusive LDR command for 8 bit value.

    \param [in]    ptr  Pointer to data
    \return             value of type uint8_t at (*ptr)
 */
#define __LDREXB(ptr)                     ((uint8_t ) __ldrex(ptr))


/** \brief  LDR Exclusive (16 bit)

    This function performs a exclusive LDR command for 16 bit values.

    \param [in]    ptr  Pointer to data
    \return        value of type uint16_t at (*ptr)
 */
#define __LDREXH(ptr)                     ((uint16_t) __ldrex(ptr))


/** \brief  LDR Exclusive (32 bit)

    This function performs a exclusive LDR command for 32 bit values.

    \param [in]    ptr  Pointer to data
    \return        value of type uint32_t at (*ptr)
 */
#define __LDREXW(ptr)                     ((uint32_t ) __ldrex(ptr))


/** \brief  STR Exclusive (8 bit)

    This function performs a exclusive STR command for 8 bit values.

    \param [in]  value  Value to store
    \param [in]    ptr  Pointer to location
    \return          0  Function succeeded
    \return          1  Function failed
 */
#define __STREXB(value, ptr)              __strex(value, ptr)


/** \brief  STR Exclusive (16 bit)

    This function performs a exclusive STR command for 16 bit values.

    \param [in]  value  Value to store
    \param [in]    ptr  Pointer to location
    \return          0  Function succeeded
    \return          1  Function failed
 */
#define __STREXH(value, ptr)              __strex(value, ptr)


/** \brief  STR Exclusive (32 bit)

    This function performs a exclusive STR command for 32 bit values.

    \param [in]  value  Value to store
    \param [in]    ptr  Pointer to location
    \return          0  Function succeeded
    \return          1  Function failed
 */
#define __STREXW(value, ptr)              __strex(value, ptr)


/** \brief  Remove the exclusive lock

    This function removes the exclusive lock which is created by LDREX.

 */
#define __CLREX                           __clrex


/** \brief  Signed Saturate

    This function saturates a signed value.

    \param [in]  value  Value to be saturated
    \param [in]    sat  Bit position to saturate to (1..32)
    \return             Saturated value
 */
#define __SSAT                            __ssat


/** \brief  Unsigned Saturate

    This function saturates an unsigned value.

    \param [in]  value  Value to be saturated
    \param [in]    sat  Bit position to saturate to (0..31)
    \return             Saturated value
 */
#define __USAT                            __usat


/** \brief  Count leading zeros

    This function counts the number of leading zeros of a data value.

    \param [in]  value  Value to count the leading zeros
    \return             number of leading zeros in value
 */
#define __CLZ                             __clz

#endif /* (__CORTEX_M >= 0x03) */




/*@}*/ /* end of group CMSIS_Core_InstructionInterface */

#pragma pop

#endif /* __CORE_CMINSTR_H */
