#ifndef USB_MIDI_USBMIDI_TYPES_H_
#define USB_MIDI_USBMIDI_TYPES_H_

#include "usblib/usblib.h"
#include "usblib/device/usbdevice.h"
#include "usb_midi_fifo.h"

#define USB_BUFFER_SIZE (512)

// status of the two directions.
typedef enum
{
	eUsbMidiStateUnconfigured, 	// not configured
	eUsbMidiStateIdle,			// no outstanding transaction remains to be completed
	eUsbMidiStateWaitData		// waiting on completion of a send or receive transaction
} tUSBMidiState;

// this is the "Device instance" structure
typedef struct {

	// base address of USB hardware
	uint32_t ui32USBBase;

	// lower level DCD code
	tDeviceInfo sDevInfo;

	// state of receive channel
	volatile tUSBMidiState iUSBMidiRxState;

	// state of transmit channel
	volatile tUSBMidiState iUSBMidiTxState;

	// device connection status.
	volatile bool bConnected;

} tUSBMidiInstance;

// This is the "device structure."
// Its main purpose is to hold the USB Buffer callback functions and data.
// Its private structure has the low-level stuff (see above).
typedef struct
{
	USBMIDIFIFO_t InEpMsgFifo;
	USBMIDIFIFO_t OutEpMsgFifo;
	tUSBMidiInstance sPrivateData;

} tUSBMidiDevice;

#endif
