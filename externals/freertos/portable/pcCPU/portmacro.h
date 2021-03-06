/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif





/* Type definitions. */
#define portCHAR        char
#define portFLOAT       float
#define portDOUBLE      double
#define portLONG        int
#define portSHORT       short
#define portSTACK_TYPE  unsigned long
#define portBASE_TYPE   long

typedef portSTACK_TYPE  StackType_t;
typedef short           BaseType_t;
typedef unsigned short  UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
    typedef unsigned portSHORT portTickType;
    #define portMAX_DELAY ( portTickType ) 0xffff
#else
    typedef unsigned portLONG portTickType;
    #define portMAX_DELAY ( portTickType ) 0xffffffff
#endif

typedef portTickType TickType_t;

/*-----------------------------------------------------------*/

/* Hardware specifics. */
#define portSTACK_GROWTH            ( -1 )
#define portTICK_PERIOD_MS          ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_PERIOD_uS          ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT          32

/*-----------------------------------------------------------*/


/* Scheduler utilities. */
extern void vPortYieldFromISR( void );
extern void vPortYield( void );

#define portYIELD()                 vPortYield()

#define portEND_SWITCHING_ISR( xSwitchRequired ) if( xSwitchRequired ) vPortYieldFromISR()
/*-----------------------------------------------------------*/


/* Critical section management. */
extern void vPortDisableInterrupts( void );
extern void vPortEnableInterrupts( void );
#define portSET_INTERRUPT_MASK()    ( vPortDisableInterrupts() )
#define portCLEAR_INTERRUPT_MASK()  ( vPortEnableInterrupts() )

extern portBASE_TYPE xPortSetInterruptMask( void );
extern void vPortClearInterruptMask( portBASE_TYPE xMask );

#define portSET_INTERRUPT_MASK_FROM_ISR()       xPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortClearInterruptMask(x)


extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portDISABLE_INTERRUPTS()    portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()     portCLEAR_INTERRUPT_MASK()
#define portENTER_CRITICAL()        vPortEnterCritical()
#define portEXIT_CRITICAL()         vPortExitCritical()
/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#define portNOP()

#define portOUTPUT_BYTE( a, b )

extern void vPortForciblyEndThread( void *pxTaskToDelete );
#define traceTASK_DELETE( pxTaskToDelete )      vPortForciblyEndThread( pxTaskToDelete )

extern void vPortAddTaskHandle( void *pxTaskHandle );
#define traceTASK_CREATE( pxNewTCB )            vPortAddTaskHandle( pxNewTCB )

/* Posix Signal definitions that can be changed or read as appropriate. */
#define SIG_SUSPEND                 SIGUSR1
#define SIG_RESUME                  SIGUSR2

/* Enable the following hash defines to make use of the real-time tick where time progresses at real-time. */
#define SIG_TICK                    SIGALRM
#define TIMER_TYPE                  ITIMER_REAL
/* Enable the following hash defines to make use of the process tick where time progresses only when the process is executing.
#define SIG_TICK                    SIGVTALRM
#define TIMER_TYPE                  ITIMER_VIRTUAL      */
/* Enable the following hash defines to make use of the profile tick where time progresses when the process or system calls are executing.
#define SIG_TICK                    SIGPROF
#define TIMER_TYPE                  ITIMER_PROF */

/* Make use of times(man 2) to gather run-time statistics on the tasks. */
extern void vPortFindTicksPerSecond( void );
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    vPortFindTicksPerSecond()       /* Nothing to do because the timer is already present. */
extern unsigned long ulPortGetTimerValue( void );
#define portGET_RUN_TIME_COUNTER_VALUE()            ulPortGetTimerValue()           /* Query the System time stats for this process. */



#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

