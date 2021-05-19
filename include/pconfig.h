#ifndef PCONFIG_H_
#define PCONFIG_H_

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"

#define SYSTICKS_PER_SECOND 100
#define SYSTICK_PERIOD_MS   (1000 / SYSTICKS_PER_SECOND)

uint32_t g_ui32SysTickCount;
uint32_t g_ui32SysClock;

USBMIDI_Message_t txmsg;
USBMIDI_Message_t rxmsg;

#endif
