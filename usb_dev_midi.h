#ifndef USB_DEV_MIDI_H_
#define USB_DEV_MIDI_H_

#include <stdint.h>
#include <stdbool.h>
#include <usbmidi_types.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/qei.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"

#include "usblib/usblib.h"
#include "usblib/device/usbdevice.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

#include "midi.h"
#include "usbmidi.h"

#define SYSTICKS_PER_SECOND 100
#define SYSTICK_PERIOD_MS   (1000 / SYSTICKS_PER_SECOND)

uint32_t g_ui32SysTickCount;
uint32_t g_ui32SysClock;

USBMIDI_Message_t txmsg;
USBMIDI_Message_t rxmsg;


void SysTickIntHandler(void) {
    g_ui32SysTickCount++;
}

void Initialize(void) {

    // Enable PORTF
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;

    // Enable PORTD
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}
    HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;

    // Configure GPIO for LEDS
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Configure GPIO for Buttons
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure USBAnalog
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // Get SysClock
    g_ui32SysClock = MAP_SysCtlClockGet();

    // Enable system tick
    MAP_SysTickPeriodSet(g_ui32SysClock / SYSTICKS_PER_SECOND);
    MAP_SysTickIntEnable();
    MAP_SysTickEnable();

}

void ConfigureUART0(void) {
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
}

void noteOn(short channel, uint8_t note, uint8_t velocity) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_NOTEON);
    txmsg.byte1 = MIDI_MSG_NOTEON | channel;
    txmsg.byte2 = note;
    txmsg.byte3 = velocity;
    USBMIDI_InEpMsgWrite(&txmsg);
}

void noteOff(short channel, uint8_t note, uint8_t velocity) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_NOTEOFF);
    txmsg.byte1 = MIDI_MSG_NOTEOFF | channel;
    txmsg.byte2 = note;
    txmsg.byte3 = velocity;
    USBMIDI_InEpMsgWrite(&txmsg);
}

void controlChange(short channel, uint8_t key, uint8_t value) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_CTRLCHANGE);
    txmsg.byte1 = MIDI_MSG_CTRLCHANGE | channel;
    txmsg.byte2 = key;
    txmsg.byte3 = value;
    USBMIDI_InEpMsgWrite(&txmsg);
}

void pitchBend(short channel, uint16_t pitch) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_PITCHBEND);
    txmsg.byte1 = MIDI_MSG_PITCHBEND | channel;
    txmsg.byte2 = pitch & 0x7F; // lsb
    txmsg.byte3 = (pitch >> 7) & 0x7F; // msb
    USBMIDI_InEpMsgWrite(&txmsg);
}

void MIDI_USB_Rx_Task(void) {
    while(USBMIDI_OutEpFIFO_Pop(&rxmsg)) {
        UARTprintf("%02x : %02x : %02x : %02x\n", rxmsg.header, rxmsg.byte1, rxmsg.byte2, rxmsg.byte3);
    }
}

void MIDI_USB_Loop_Task(void) {
    while(USBMIDI_OutEpFIFO_Pop(&rxmsg)) {
        if(rxmsg.header!=0) {
            USBMIDI_InEpMsgWrite(&rxmsg);
            MAP_SysCtlDelay(10000);
        }
    }
}

#endif
