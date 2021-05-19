#ifndef USB_MIDI_USBMIDI_HANDLERS_H_
#define USB_MIDI_USBMIDI_HANDLERS_H_

#include <stdint.h>
#include <stdbool.h>
#include "usblib/device/usbdevice.h"

void HandleRequests(void *pvMidiDevice, tUSBRequest *pUSBRequest);
void HandleConfigChange(void *pvMidiDevice, uint32_t ui32Info);
void HandleDisconnect(void *pvMidiDevice);
void HandleEndpoints(void *pvMidiDevice, uint32_t ui32Status);
void HandleSuspend(void *pvMidiDevice);
void HandleResume(void *pvMidiDevice);
void USBMidiTickHandler(void *pvUSBMidiDevice, uint32_t ui32TimemS);

#endif
