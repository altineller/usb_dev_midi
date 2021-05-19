/**
 * usb_midi_fifo.c
 *
 * Implement a software FIFO for USB MIDI packets.
 *
 *  Created on: Oct 28, 2019
 *      Author: apeters
 *  Mods:
 *  2019-10-30 ASP: support multiple instances of a FIFO by requiring a pointer to the FIFO structure
 *  	for each function call. All FIFOs are the same size.
 */

#include <stdint.h>
#include <stdbool.h>
#include "usb_midi.h"
#include "usb_midi_fifo.h"

/*
 * Initialize the MIDI message FIFO.
 */
void USBMIDIFIFO_Init(USBMIDIFIFO_t *fifo)
{
	fifo->head = 0;
	fifo->tail = 0;
	fifo->count = 0;
} // MIDIFIFO_Init()

/**
 * Push a new message onto the FIFO.
 * @bug Do what in case of overflow?
 * @param msg The MIDI message to push onto the FIFO.
 */
void USBMIDIFIFO_Push(USBMIDIFIFO_t *fifo, USBMIDI_Message_t *msg)
{
	fifo->msg[fifo->head] = *msg;
	fifo->head++;
	if (fifo->head >= MIDI_USB_FIFO_SIZE) {
		fifo->head = 0;
	}
	fifo->count++;
} // MIDIFIFO_Push()

/**
 * Pop a message from the USB MIDI FIFO.
 * This fetches a message that was sent from the host on an OUT endpoint.
 * @returns true if we actually popped something, else false if the FIFO was
 * empty.
 * @param msg: this is the message popped from the FIFO.
 */
bool USBMIDIFIFO_Pop(USBMIDIFIFO_t *fifo, USBMIDI_Message_t *msg)
{
	if (fifo->count == 0)
		// nothing to pop, so caller doesn't parse any message.
		return false;

	*msg = fifo->msg[fifo->tail];
	fifo->tail++;
	if (fifo->tail >= MIDI_USB_FIFO_SIZE) {
		fifo->tail = 0;
	}
	fifo->count--;

	return true;
} // MIDIFIFO_Pop()
