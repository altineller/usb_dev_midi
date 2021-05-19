/*
 * usbmidi.h
 *
 *  Created on: Jul 1, 2020
 *      Author: andy
 */

#ifndef USB_MIDI_USBMIDI_H_
#define USB_MIDI_USBMIDI_H_

#include <stdint.h>

/**
 * Initialize the USB MIDI device.
 */
void USBMIDI_Init(uint32_t index);

/**
 * return connection status.
 */
bool USBMIDI_IsConnected(void);

/**
 * Functions to access the message FIFOs.
 *
 * Pop the OUT Endpoint FIFO, which returns messages sent to us from the host.
 */
bool USBMIDI_OutEpFIFO_Pop(USBMIDI_Message_t *msg);

/**
 * Push a new message to the outgoing (IN Endpoint) fifo.
 */
void USBMIDI_InEpMsgWrite(USBMIDI_Message_t *msg);

/**
 * Check to see if transmit (IN endpoint) message FIFO has things to send.
 * If it does, repeatedly pop the FIFO and write the message bytes into
 * a buffer which then gets loaded into the endpoint buffer and transmitted.
 */
void USBMIDI_InEpSendMessages(void);

#endif /* USB_MIDI_USBMIDI_H_ */
