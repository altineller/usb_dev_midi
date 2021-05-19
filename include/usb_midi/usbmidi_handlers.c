/*
 * usbmidi_handlers.c
 *
 *  Created on: Feb 3, 2020
 *      Author: andy
 */

#include <stdbool.h>
#include <stdint.h>

#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usblibpriv.h"

#include "usbmidi_handlers.h"
#include "usbmidi_types.h"
#include "usbmidi.h"

#include "pconfig.h"

/**
 * USB MIDI does define some class-specific requests, which we do not handle.
 */
void HandleRequests(void *pvMidiDevice, tUSBRequest *pUSBRequest)
{
	USBDCDStallEP0(0);
}

/**
 * Callback invoked when data are available on an OUT endpoint or to present data to an IN endpoint.
 *
 * Called by the USB stack for any activity involving one of our endpoints
 * other than EP0.  This function is a fan out that merely directs the call to
 * the correct handler depending upon the endpoint and transaction direction
 * signaled in ui32Status.
 *
 * \param pvMidiDevice is a pointer to the device structure that holds everything we
 * 	need to know about this device.
 * \param ui32Status here holds the return from a call to MAP_USBIntStatusEndpoint().
 *    Upper 16 bits of ui32Status hold the USB Receive Interrupt status.
 *    Lower 16 bits of ui32Status hold the USB Transmit Interrupt status.
 *    For each of those statuses, bits [7:0] are bit mask. A bit set means that the
 *    	corresponding endpoint's interrupt is active. Bit 0, for EP0, will always be 0.
 *
 *
 * The Endpoint Status is like the interrupt status sent in.
 *   The upper 16 bits hold the receive endpoint status, and the lower 16 bits
 *   hold the transmit endpoint status. This seems rather daft, I think.
 *   Anyway, get the status before determining the source of the interrupts.
 */
void HandleEndpoints(void *pvMidiDevice, uint32_t ui32Status)
{
	tUSBMidiDevice *psUsbMidiDevice;
	tUSBMidiInstance *psInst;
	uint32_t ui32EPStatus;
	uint32_t bytecount;
	uint8_t buf[64];	// read endpoint data into this, which is max packet size
	uint8_t *pbuf;
	USBMIDI_Message_t usbmep;			// build a message into this.

	ASSERT(pvMidiDevice != 0);

	// Get a pointer to our device record.
	psUsbMidiDevice = (tUSBMidiDevice *) pvMidiDevice;
	psInst = &psUsbMidiDevice->sPrivateData;

	// Get the endpoint status to see why we were called.
	ui32EPStatus = MAP_USBEndpointStatus(USB0_BASE, USB_EP_1);
    // Clear the status bits.
    MAP_USBDevEndpointStatusClear(USB0_BASE, USB_EP_1, ui32EPStatus);

    // if Interrupt From OUT Endpoint.
	// Data are coming in from the host, and we should handle them.
	if( ui32Status & USB_INTEP_DEV_OUT_1 )
	{
		// Check to see if the OUT endpoint has data for us.
		// I suppose that checking the data available amounts to the same thing.
		if( ui32EPStatus & USB_DEV_RX_PKT_RDY )
		{
			// Data are being sent to us from the host.
			// Get all bytes in buffer.
			bytecount = MAP_USBEndpointDataAvail(USB0_BASE, USB_EP_1);
			MAP_USBEndpointDataGet(USB0_BASE, USB_EP_1, buf, &bytecount);
			// now take the contents of buf and make a bunch of MIDI packets
			// and push them to the incoming data FIFO.
			pbuf = buf;
			while( bytecount ) {
				usbmep.header = *pbuf++;
				usbmep.byte1  = *pbuf++;
				usbmep.byte2  = *pbuf++;
				usbmep.byte3  = *pbuf++;
				USBMIDIFIFO_Push(&psUsbMidiDevice->OutEpMsgFifo, &usbmep);
				bytecount -= 4;
			}

			// ack the data, thus freeing the host to send the next packet.
			MAP_USBDevEndpointDataAck(USB0_BASE, USB_EP_1, true);
		}
    }

	// Interrupt From IN Endpoint?
	// This is set after a packet was sent to the host.
	// Check to see if there are more MIDI messages to send, and do so if there are.
	if( ui32Status & USB_INTEP_DEV_IN_1 )
	{
		// Indicate that the endpoint is ready for new data.
	    psInst->iUSBMidiTxState = eUsbMidiStateIdle;
		USBMIDI_InEpSendMessages();
	}
}

// This should indicate that we are attached to the bus
void HandleConfigChange(void *pvMidiDevice, uint32_t ui32Status) {

	tUSBMidiDevice *psUSBMidiDevice;
	tUSBMidiInstance *psInst;

	psUSBMidiDevice = (tUSBMidiDevice *) pvMidiDevice;
	psInst = &psUSBMidiDevice->sPrivateData;
    psInst->bConnected = true;
    psInst->iUSBMidiRxState = eUsbMidiStateIdle;
    psInst->iUSBMidiTxState = eUsbMidiStateIdle;

	USBMIDIFIFO_Init(&psUSBMidiDevice->InEpMsgFifo);
	USBMIDIFIFO_Init(&psUSBMidiDevice->OutEpMsgFifo);

}

void HandleDisconnect(void *pvMidiDevice) {

	tUSBMidiDevice *psUSBMidiDevice;
	tUSBMidiInstance *psInst;

	psUSBMidiDevice = (tUSBMidiDevice *) pvMidiDevice;
	psInst = &psUSBMidiDevice->sPrivateData;
    psInst->bConnected = false;

}

void HandleSuspend(void *pvMidiDevice) {
}

void HandleResume(void *pvMidiDevice) {
}

