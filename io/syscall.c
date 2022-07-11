/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 1995 - 2021 SEGGER Microcontroller GmbH             *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       SEGGER RTT * Real Time Transfer for embedded targets         *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* SEGGER strongly recommends to not make any changes                 *
* to or modify the source code of this software in order to stay     *
* compatible with the RTT protocol and J-Link.                       *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* condition is met:                                                  *
*                                                                    *
* o Redistributions of source code must retain the above copyright   *
*   notice, this condition and the following disclaimer.             *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************
*                                                                    *
*       RTT version: 6.98b                                           *
*                                                                    *
**********************************************************************

---------------------------END-OF-HEADER------------------------------
File    : SEGGER_RTT_Syscalls_GCC.c
Purpose : Low-level functions for using printf() via RTT in GCC.
          To use RTT for printf output, include this file in your
          application.
Revision: $Rev: 20755 $
----------------------------------------------------------------------
*/
#if (defined __GNUC__) && !(defined __SES_ARM) && !(defined __CROSSWORKS_ARM) && !(defined __ARMCC_VERSION) && !(defined __CC_ARM)

#ifdef USE_UART_PRINT
#include "bsp_uart.h"
#endif
#ifdef USE_RTT_PRINT
#include "SEGGER_RTT.h"
#endif
// OR:

// Add syscalls.c with GCC

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
#ifdef USE_UART_PRINT
  uart_send_byte((const char *)&ch, 1);
#endif
#ifdef USE_RTT_PRINT
  SEGGER_RTT_Write(0, (const char *)&ch, 1);
  return ch;
#endif
}

/*********************************************************************
 *
 *       Types
 *
 **********************************************************************
 */
//
// If necessary define the _reent struct
// to match the one passed by the used standard library.
//
struct _reent;

/*********************************************************************
 *
 *       Function prototypes
 *
 **********************************************************************
 */
_ssize_t _write(int file, const void *ptr, size_t len);
_ssize_t _write_r(struct _reent *r, int file, const void *ptr, size_t len);

/*********************************************************************
 *
 *       Global functions
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       _write()
 *
 * Function description
 *   Low-level write function.
 *   libc subroutines will use this system routine for output to all files,
 *   including stdout.
 *   Write data via RTT.
 */
_ssize_t _write(int file, const void *ptr, size_t len)
{
#ifdef USE_UART_PRINT
  uart_send_byte(ptr, len);
#endif

#ifdef USE_RTT_PRINT
  (void)file; /* Not used, avoid warning */
  SEGGER_RTT_Write(0, ptr, len);

#endif
  return len;
}

/*********************************************************************
 *
 *       _write_r()
 *
 * Function description
 *   Low-level reentrant write function.
 *   libc subroutines will use this system routine for output to all files,
 *   including stdout.
 *   Write data via RTT.
 */
_ssize_t _write_r(struct _reent *r, int file, const void *ptr, size_t len)
{
  _write(file, ptr, len);
  return len;
}

#endif
/****** End Of File *************************************************/
