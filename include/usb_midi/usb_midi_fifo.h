/*
 * usb_midi_fifo.h
 *
 * This implements a FIFO for USB MIDI messages that are received from the host
 * via USB OUT transactions.
 *
 * The messages are pushed in a callback.
 * They are popped in the main program loop.
 *
 *  Created on: Oct 28, 2019
 *      Author: apeters
 *  Mods:
 *  2019-10-30 ASP: support multiple instances of a FIFO by requiring a pointer to the FIFO structure
 *  	for each function call. All FIFOs are the same size. The size is defined as MIDI_USB_FIFO_SIZE.
 */

#ifndef USB_MIDI_USB_MIDI_FIFO_H_
#define USB_MIDI_USB_MIDI_FIFO_H_

#include <stdint.h>
#include <stdbool.h>

#include "usb_midi.h"

#ifndef MIDI_USB_FIFO_SIZE
#define MIDI_USB_FIFO_SIZE 64
#endif

/*
 * Define a software FIFO for the MIDI messages.
 */
typedef struct {
	uint8_t head;									/*!< Index of the head of the FIFO  */
	uint8_t tail;									/*!< Index of the tail of the FIFO  */
	uint8_t count;									/*!< Number of messages in the FIFO */
	USBMIDI_Message_t msg[MIDI_USB_FIFO_SIZE];		/*!< the buffer */
} USBMIDIFIFO_t;

/**
 * Initialize the MIDI message FIFO.
 * Clear the contents to make debug easier.
 */
void USBMIDIFIFO_Init(USBMIDIFIFO_t *fifo);

/**
 * Push a new message onto the FIFO.
 * \param[in,out] msg: pointer to a USB MIDI message structure.
 * \bug Do what in case of overflow?
 */
void USBMIDIFIFO_Push(USBMIDIFIFO_t *fifo, USBMIDI_Message_t *msg);

/**
 * Pop a message from the MIDI Message FIFO.
 * \returns true if we actually popped something, else false if the FIFO was
 * empty.
 * \param[in,out] msg: The message is returned in the argument.
 */
bool USBMIDIFIFO_Pop(USBMIDIFIFO_t *fifo, USBMIDI_Message_t *msg);

#endif /* USB_MIDI_USB_MIDI_FIFO_H_ */
