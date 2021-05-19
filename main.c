/*
 * USB MIDI for TM4C123
 *
 * Copyright 2021 Stephan Bourgeois
 * Ported to TM4C123 by Can Altineller
 * Originally written by Andy Peters
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 */

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

#include "pconfig.h"
#include "midi.h"
#include "usbmidi.h"
#include "midi_usb_rx_task.h"
#include "main.h"

void SysTickIntHandler(void) {
    g_ui32SysTickCount++;
}

int main(void) {

    // initialize tiva-c @ 80mhz
    MAP_FPUEnable();
    MAP_FPULazyStackingEnable();
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    Initialize();

    // initialize USB
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    // initialize USB MIDI Device
    USBMIDI_Init(0);

    // initialize master interrput
    MAP_IntMasterEnable();

    ConfigureUART0();

    bool prev_state = false;
    bool connected = false;

    while(1) {

        connected = USBMIDI_IsConnected();

    	if(connected) {
    		if(prev_state == false) { prev_state = true; }
    	} else {
    		if(prev_state == true) { prev_state = false; }
    	}

    	// will receive MIDI notes and print them on serial
    	MIDI_USB_Rx_Task();

    	// will receive MIDI notes and echo them back
    	// MIDI_USB_Loop_Task();

        // note on
        noteOn(0, 0x40, 0x44);
        MAP_SysCtlDelay(8000000);

        // note off
        noteOff(0, 0x40, 0x44);
        MAP_SysCtlDelay(8000000);

        /*
        // control change
        controlChange(0x1, 0x0, 128);
        MAP_SysCtlDelay(8000000);

        // pitch bend
        pitchBend(0x1, 64);
        MAP_SysCtlDelay(8000000);
        */

    }

}
