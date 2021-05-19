/**
 * \file usb_midi.h
 *
 *  Created on: Feb 1, 2019
 *      Author: andy
 *  Modification:
 *  2019-04-04 andy	Add more descriptor definitions.
 *  2019-04-30 andy Add prototype for midi message fifo pop.
 *  2019-05-21 andy No Almost Full flag.
 *  2019-06-10 andy xdata space for the messages, attempt at doxygen.
 *  2019-10-30 andy for EFM32.
 */

#ifndef USB_MIDI_H_
#define USB_MIDI_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * \typedef USBMIDI_Message_t
 *
 * USB-MIDI Event Packets.
 * From USB Device Class Specification for MIDI Devices, Section 4.
 *
 * MIDI data are transferred in 32-bit USB-MIDI packets.
 * The header is byte 0, which is transmitted first.
 * The upper nybble of the event is the MIDI Cable Number, which is the number
 * assignment of the Embedded MIDI Jack associated with the endpoint transferring
 * the data.
 * The lower nybble of the event is the Code Index Number (CIN), which indicates the
 * classification of the bytes in the MIDI_x fields.
 */
typedef struct
{
	uint8_t header;		//!< Cable Number, Code Index Number
	uint8_t byte1;		//!< MIDI_0
	uint8_t byte2;		//!< MIDI_1
	uint8_t byte3;		//!< MIDI_2
} USBMIDI_Message_t;

/**
 * MIDI Code Index Number Classifications.
 * Please refer to the USB MIDI Device Class Spec, table 4-1.
 * This is the lower nybble of the header (byte 0) of the USB-MIDI Event Packet)
 */

/*! \def Reserve for future expansion */
#define USB_MIDI_CIN_MISC 			0x0
/*! \def Cable Events, reserved for future expansion */
#define USB_MIDI_CIN_CABLEEVENTS 	0x1
/*! \def Two-byte System Common messages */
#define USB_MIDI_CIN_SYSCOM2 		0x2
/*! \def Three-bytes System Common messages. */
#define USB_MIDI_CIN_SYSCOM3		0x3
/*! \def SysEx starts or continues */
#define USB_MIDI_CIN_SYSEXSTART		0x4
/*! \def Single-byte System Common or SysEx ends */
#define USB_MIDI_CIN_SYSEND1		0x5
/*! \def SysEx ends with following two bytes */
#define USB_MIDI_CIN_SYSEND2		0x6
/*! \def SysEx ends with following three bytes */
#define USB_MIDI_CIN_SYSEND3 		0x7
/*! \def Note Off */
#define USB_MIDI_CIN_NOTEOFF 		0x8
/*! \def Note On */
#define USB_MIDI_CIN_NOTEON 		0x9
/*! \def Poly KeyPress */
#define USB_MIDI_CIN_POLYKEYPRESS 	0xA
/*! \def Control change */
#define USB_MIDI_CIN_CTRLCHANGE 	0xB
/*! \def Program change */
#define USB_MIDI_CIN_PROGCHANGE		0xC
/*! \def Channel Pressure */
#define USB_MIDI_CIN_CHANPRESSURE	0xD
/*! \def Pitchbend change */
#define USB_MIDI_CIN_PITCHBEND		0xE
/*! \def Single Byte */
#define USB_MIDI_CIN_SINGLEBYTE		0xF

/**
 * \def Construct a MIDI packet header from a given MIDI command and a virtual MIDI cable index.
 */
#define USB_MIDI_HEADER(virtualcable, command)  (((virtualcable) << 4) | command)

/**
 * \def extract the cable number from a given event ID.
 */
#define USB_MIDI_CABLE_NUMBER(event) (event >> 4)

/**
 * \def extract the Code Index Number from a given event ID.
 */
#define USB_MIDI_CODE_INDEX_NUMBER(event) (event & 0x0F)

/** \enum for the possible MIDI jack types in a MIDI device jack descriptor. */
//	enum MIDI_JackTypes_t
//	{
#define		MIDI_JACKTYPE_Embedded (0x01) /**< MIDI class descriptor jack type value for an embedded (logical) MIDI input or output jack. */
#define		MIDI_JACKTYPE_External (0x02) /**< MIDI class descriptor jack type value for an external (physical) MIDI input or output jack. */
//	};

/** \enum for  the MS class-specific interface descriptor subtypes. */
//	enum MIDI_CS_IF_Subtypes_t
//	{
#define	MIDI_CS_IF_HEADER   (0x01)
#define	MIDI_CS_IF_IN_JACK  (0x02)
#define	MIDI_CS_IF_OUT_JACK (0x03)
#define	MIDI_CS_IF_ELEMENT  (0x04)
//	};

/**
 * \def Class-specific endpoint descriptor type
 */
#define USB_CS_ENDPOINT_DESCRIPTOR 0x25

/**
 * \def Class-specific endpoint descriptor subtype
 */
#define USB_MIDI_CS_EP_MS_GENERAL 0x01

/**
 * \typedef struct USB_MIDI_CS_AC_IF_Descriptor_t
 * From USB Device Class Specification for MIDI Devices, Section 6.
 *
 * Class-Specific MIDI Audio Control Interface Header Descriptor. We will assume
 * that there will only be one MIDI Streaming interface in the design, so
 * when used bInCollection should be set to 1 and BaInterfaceNr (normally an array)
 * should be set to the number of this interface.
 */
typedef struct
{
	uint8_t bLength;			//!< Size of this descriptor; in bytes (9)
	uint8_t bDescriptorType;	//!< CS_INTERFACE descriptor type
	uint8_t bDescriptorSubtype;	//!< MS_HEADER descriptor subtype
	uint16_t bcdADC;			//!< Audio Class spec revision in BCD; currently 01.00
	uint16_t wTotalLength;		//!< total number of bytes returned for the class-specific MS IF descriptor.
								//!< Includes the combined length of this descriptor and all the
								//!< Jack and Element descriptors.
	uint8_t bInCollection;		//!< Number of MIDI Streaming Interfaces
	uint8_t baInterfaceNr;		//!< the interface number of the first (and only) MS IF
} USB_MIDI_CS_AC_IF_Descriptor_t;

#define USB_MIDI_CS_AC_IF_DESC_SIZE (9)

/**
 * \typedef USB_MIDI_CS_MS_IF_Descriptor_t
 * Class-specific MIDI Streaming Interface Header Descriptor.
 */
typedef struct
{
	uint8_t bLength;			//!< Size of this descriptor; in bytes (7)
	uint8_t bDescriptorType;	//!< CS_INTERFACE descriptor type
	uint8_t bDescriptorSubtype;	//!< MS_HEADER descriptor subtype
	uint16_t BcdADC;			//!< revision of this class spec, currently 01.00
	uint16_t wTotalLength;		//!< total size of class-specific descriptors
} USB_MIDI_CS_MS_IF_Descriptor_t;

#define USB_MIDI_CS_MS_IF_DESC_SIZE (7)

/**
 * \typedef USB_MIDI_In_Jack_Descriptor_t
 * MIDI IN Jack Descriptor
 */
typedef struct
{
	uint8_t bLength;			//!< Size of this descriptor; in bytes (6)
	uint8_t bDescriptorType;	//!< CS_INTERFACE descriptor type
	uint8_t bDescriptorSubtype;	//!< MIDI_IN_JACK descriptor subtype
	uint8_t bJackType;			//!< EMBEDDED or EXTERNAL
	uint8_t bJackID;			//!< constant identifying this Jack within the function
	uint8_t iJack;				//!< index of a string descriptor describing this jack
} USB_MIDI_In_Jack_Descriptor_t;

#define USB_MIDI_IN_JACK_DESC_SIZE (6)

/**
 * \def USB_MIDI_OUT_JACK_DESCRIPTOR
 * MIDI Out Jack Descriptor.
 * This is a macro, and at instantiation time the variable NumSources must be set to
 * the number of sources which are inputs to this jack. This value will fill the bNrInputPins
 * field as well as being used to determine the size of two arrays and the entire structure.
 * The structure is 7 bytes plus however many bytes are needed by the source ID and pin arrays.
 */
#define USB_MIDI_OUT_JACK_DESCRIPTOR_SIZE(NumSources) (7 + 2*NumSources)
#define USB_MIDI_OUT_JACK_DESCRIPTOR(NumSources)	\
	struct											\
	{												\
		uint8_t bLength;							\
		uint8_t bDescriptorType;					\
		uint8_t bDescriptorSubtype;					\
		uint8_t bJackType;							\
		uint8_t bJackID;							\
		uint8_t bNrInputPins;						\
		uint8_t baSourceID[NumSources];				\
		uint8_t baSourcePin[NumSources];			\
		uint8_t iJack;								\
	}

/**
 * \def USB_MIDI_CS_STREAMING_BULK_ENDPOINT
 * MIDIStreaming Endpoint Descriptors
 * USB MIDI Spec section 6.2
 *
 * Class-Specific MIDI Streaming Bulk Data Endpoint Descriptor.
 * This is a macro, and at instantiation time the variable NumJacks must be set to
 * the number of embedded jacks associated with this endpoint. This value must be
 * used for the bNumEmbMIDIJack member. The structure is 4 bytes plus the number of
 * jacks.
 */
#define USB_MIDI_CS_STREAMING_BULK_ENDPOINT_SIZE(NumJacks) (4 + NumJacks)
#define USB_MIDI_CS_STREAMING_BULK_ENDPOINT(NumJacks)	\
	struct												\
	{													\
		uint8_t bLength;								\
		uint8_t bDescriptorType;						\
		uint8_t bDescriptorSubtype;						\
		uint8_t bNumEmbMIDIJack;						\
		uint8_t baAssocJackID[NumJacks];				\
	}

/*
 * Initialize things that need to be initialized.
 * As of now, it's just the receive message FIFO.
 */
//void USBMIDI_Init(void);

/**
 * Check the receive (USB MIDI OUT) FIFO to see if any messages are waiting.
 * If so, the message will be in msg and the function returns true.
 * (This function hides the static msgfifo from the larger program.)
 */
bool USBMIDI_RxFifoPop(USBMIDI_Message_t *msg);

/*
 * Start a read (OUT) transfer on our MIDI Streaming endpoint.
 */
void USBMIDI_Read(void);

/*
 * Start a write (IN) transfer on our MIDI Streaming Endpoint.
 * There is no callback for writes, at least not until we implement a FIFO for it.
 * This blocks if the endpoint is busy.
 */
void USBMIDI_InEpMsgWrite(USBMIDI_Message_t *msg);

#endif /* USB_MIDI_H_ */
