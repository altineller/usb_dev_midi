#ifndef MIDI_USB_RX_TASK_H_
#define MIDI_USB_RX_TASK_H_

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

#include "usb_midi.h"
#include "usb_midi_fifo.h"
#include "midi_usb_rx_task.h"
#include "usbmidi.h"

#include "pconfig.h"

void MIDI_USB_Rx_Task(void);
void MIDI_USB_Loop_Task(void);

#endif
