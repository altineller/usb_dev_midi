/*
 * descriptors.h
 *
 *  Created on: Jan 30, 2020
 *      Author: andy
 */

#ifndef DESCRIPTORS_H_
#define DESCRIPTORS_H_

#include <stdlib.h>
#include "usblib/device/usbdevice.h"
#include "usblib/usblib.h"

/**
 * \def Class-specific endpoint descriptor type
 */
#define USB_CS_ENDPOINT_DESCRIPTOR 0x25

/**
 * \def Class-specific endpoint descriptor subtype
 */
#define USB_MIDI_CS_EP_MS_GENERAL 0x01

/**
 * \def Size of class-specific audio-control interface descriptor.
 */
#define USB_MIDI_CS_AC_IF_DESC_SIZE (9)

/**
 * \def Size of class-specific MIDI Streaming Interface Descriptor.
 */
#define USB_MIDI_CS_MS_IF_DESC_SIZE (7)

/** \enum for  the MS class-specific interface descriptor subtypes. */
//  enum MIDI_CS_IF_Subtypes_t
//  {
#define MIDI_CS_IF_HEADER   (0x01)
#define MIDI_CS_IF_IN_JACK  (0x02)
#define MIDI_CS_IF_OUT_JACK (0x03)
#define MIDI_CS_IF_ELEMENT  (0x04)
//  };

/** \enum for the possible MIDI jack types in a MIDI device jack descriptor. */
//  enum MIDI_JackTypes_t
//  {
#define     MIDI_JACKTYPE_Embedded (0x01) /**< MIDI class descriptor jack type value for an embedded (logical) MIDI input or output jack. */
#define     MIDI_JACKTYPE_External (0x02) /**< MIDI class descriptor jack type value for an external (physical) MIDI input or output jack. */
//  };

#define USB_MIDI_IN_JACK_DESC_SIZE (6)

#define USB_MIDI_OUT_JACK_DESCRIPTOR_SIZE(NumSources) (7 + 2*NumSources)

#define USB_MIDI_CS_STREAMING_BULK_ENDPOINT_SIZE(NumJacks) (4 + NumJacks)

/**
 * We use these endpoints.
 * These are different than the parameters for some of the API functions, which have the
 * EP number shifted left a nybble.
 */
#define USBMIDI_MS_EP_IN  (0x01)
#define USBMIDI_MS_EP_OUT (0x01)

#endif /* DESCRIPTORS_H_ */
