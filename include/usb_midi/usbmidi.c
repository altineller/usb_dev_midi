/*
 * usb_midi.c
 *
 *  Created on: Jan 31, 2020
 *      Author: andy
 *
 *  MOD HISTORY.
 *  2020-02-25 andy. We can combine all of the config descriptor parts into one big lump.
 *  	This means the Audio Control and the MIDI Streaming interfaces are combined into
 *  	one.
 *
 *
 *  The descriptors require some header/etc types for the API to work.
 *
 *  So there is:
 *
 *  a) Device Descriptor. No header or other needed. This is directly included
 *      in the tDeviceInfo instance.
 *
 *  b) Configuration Descriptor. This includes:
 *      1. Config Descriptor.
 *          Needs a tConfigSection wrapper.
 *      2. Interface Descriptor for Audio Control and MIDI Streaming interfaces.
 *          Needs a tConfigSection wrapper.
 *
 *  c) Combine the two tConfigSections defined above into an array of
 *      tConfigSections.
 *
 *  d) Put a tConfigHeader wrapper around that array of sections.
 *
 *  e) Because the tDeviceInfo structure wants an array of tConfigHeaders,
 *      make one.
 *
 *
 *****************************************************************************
 *
 * Interrupt handlers.
 *
 * The USB0 interrupt invokes USB0DeviceIntHandler() (in usbdhandler.c) and
 * that calls USBDeviceIntHandlerInternal() with the interrupt status
 * from MAP_USBIntStatusControl().
 *
 * USBDeviceIntHandler() invokes callbacks for the various interrupt conditions,
 * including:
 *
 * HandleEndpoint() -- this is the handler invoked by the USB interrupt when
 * there was activity on an endpoint.
 *
 * In turn, HandleEndpoint() calls ProcessDataToHost() or ProcessDataFromHost().
 *
 * ProcessDataToHost() is called after a transmit (feeding data to a USB IN transaction)
 * is finished.
 *****************************************************************************
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <usbmidi_types.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/usblibpriv.h"
#include "usblib/usbaudio.h"
#include "usblib/device/usbdevice.h"

#include "usbmidi.h"
#include "usbmidi_types.h"
#include "usbmidi_descriptors.h"
#include "usbmidi_handlers.h"
#include "pconfig.h"

/**
 * Device Descriptor.
 */
const uint8_t g_pui8AudioMIDIDeviceDescriptor[] =
{
    18,                   // bLength
    USB_DTYPE_DEVICE,     // bDescriptorType
    USBShort(0x0110),     // bcdUSB
    0x00,               // bDeviceClass,  defined at interface level
//	0xEF,				  // bDeviceClass: miscellaneous class device for IAD
    0x00,               // bDeviceSubClass, defined at interface level
//	0x02,				  // bDeviceSubClass: Common class for IAD
    0x00,               // bDeviceProtocol, defined at interface level
//	0x01,				  // bDeviceProtocol: Interface Association Descriptor.
    MAX_PACKET_SIZE_EP0,  // bMaxPacketSize0
    USBShort(USB_VID_TI_1CBE), // idVendor
    USBShort(USB_PID_BULK),    // idProduct
    USBShort(0x0100),     // bcdDevice
    1,                    // iManufacturer
    2,                    // iProduct
    3,                    // iSerialNumber
    1                     // bNumConfigurations
};

/**
 * Configuration descriptor.
 * To meet the needs of the API, we break up the descriptor into constituent parts:
 *
 * Config Descriptor Header
 * Audio Control Interface Descriptor
 * MIDI Streaming Interface Descriptor.
 *
 * (The AC IF has a standard descriptor and a  class-specific IF descriptor, the latter
 * of which is considered part of the standard interface descriptor for purposes of this
 * API design. Likewise the MS IF has a standard descriptor and class-specific descriptor.
 * The MS CS IF includes the jack descriptors.)
 *
 * These four descriptors need to be made into tConfigSections, and those sections
 * get put into an array of sections.
 *
 * Then a tConfigHeader is made from that array of sections.
 *
 * Because the API allows for more than one configuration, an array of tConfigHeaders
 * is made from that one. The array has only one entry.
 */

/**
 * Configuration Descriptor Header.
 */
#define USBMIDI_INTERFACEDESCRIPTOR_SIZE (129)
//#define USBMIDI_INTERFACEDESCRIPTOR_SIZE (137)

const uint8_t g_pui8MidiDescriptor[] =
{
    9,                         // bLength:         Size of this descriptor
    USB_DTYPE_CONFIGURATION,   // bDescriptorType: Type of descriptor
    USBShort(USBMIDI_INTERFACEDESCRIPTOR_SIZE),             // wTotalLength:    Total size of full config descriptor, will be patched
    2,                         // bNumInterfaces:  # of interfaces, Audio Control and MIDI Streaming
    1,                         // bConfigurationValue: this is config #1
    5,                         // iConfiguration:  index to descriptive string
    USB_CONF_ATTR_SELF_PWR,    // bmAttrib:        Self-powered
    50                         // bMaxPower:       100 mA from the bus
};

/**
 * Interface association descriptor, for USB 2 composite devices.
 */
const uint8_t g_pui8IADMidiDescriptor[] =
{
	8,							// size of the descriptor
	USB_DTYPE_INTERFACE_ASC,	// this is an IAD
	0x00,						// default starting interface is 0
	0x02,						// # of interfaces in this assoc (AC, MS)
	USB_CLASS_AUDIO,            // The device class for this association.
	USB_SUBCLASS_UNDEFINED,     // The device subclass for this association.
	USB_PROTOCOL_UNDEFINED,     // The protocol for this association.
	0                           // The string index for this association.
};

// header for the IAD.
const tConfigSection g_sIADMidiConfigSection =
{
    sizeof(g_pui8IADMidiDescriptor),
    g_pui8IADMidiDescriptor
};

// define the MIDI Jacks for this particular design

#define JACK1_EMBIN  0x01
#define JACK2_EXTOUT 0x02
#define JACK3_EXTIN  0x03
#define JACK4_EMBOUT 0x04
#define JACK5_EMBIN  0x05
#define JACK6_EXTOUT 0x06
#define JACK7_EXTIN  0x07
#define JACK8_EMBOUT 0x08

/**
 * The two interface descriptors.
 *
 * Audio Control Interface Descriptor.
 * This includes the standard interface descriptor and the
 * class-specific Audio Control descriptor.
 */
const uint8_t g_pui8AudioMidiControlInterface[] =
{
	/*
	 * This is the Audio Control Interface Descriptor.
	 */
	// Standard Interface Descriptor for the Audio Control Interface
    9,                     // bLength:             size of this descriptor                              1
    USB_DTYPE_INTERFACE,   // bDescriptorType                                                           2
    0,                     // bInterfaceNumber:    AC interface is #0                                   3
    0,                     // bAlternateSetting:   none, not used                                       4
    0,                     // bNumEndpoints:       this interface uses only the control endpoint        5
    USB_CLASS_AUDIO,       // bInterfaceClass:     this is an audio device								6
    USB_ASC_AUDIO_CONTROL, // bInterfaceSubClass:  this is an audio control interface					7
    0,                     // bInterfaceProtocol:  none													8
    0,                     // iInterface:          no string											9

    // Class-specific Audio Control Interface Descriptor:
    9,                    		// bLength:             size of this descriptor							10
    USB_DTYPE_CS_INTERFACE,    	// bDescriptorType:   Class-Specific Interface							11
    USB_MIDI_CS_EP_MS_GENERAL, 	// bDescriptorSubType:  MIDI Class-specific Interface					12
    USBShort(0x0100),      		// bcdADC:              version of this class							14
    USBShort(0x0009),      		// wTotalLength:        size of this entire descriptor					16
    1,                     		// bInCollection:       Number of MIDI streaming interfaces				18
    1                     		// baInterfaceNr:       MIDI streaming IF belongs to this AC IF			19
};

/*****
 * This is the MIDI Streaming interface descriptor.
 */
const uint8_t g_pui8MidiStreamInterface[] =
{
    // Standard Interface descriptor for a MIDI streaming interface
    9,                         // bDescriptorSize														20
    USB_DTYPE_INTERFACE,       // bDescriptorType														21
    1,                         // bInterfaceNumber, MIDI streaming is #1								22
    0,                         // bAlternateSetting, there is none										23
    2,                         // bNumEndpoints, we use two												24
    USB_CLASS_AUDIO,           // bInteraceClass, Audio Class											25
    USB_ASC_MIDI_STREAMING,    // bInterfaceSubClass													26
    0,                         // bInterfaceProtocol, none												27
    0,                         // iInterface, no string													28

    // Class-specific descriptor for the MIDI streaming interface.
    USB_MIDI_CS_MS_IF_DESC_SIZE,   // bLength, size of this class-specific interface descriptor header	29
    USB_DTYPE_CS_INTERFACE,        // bDescriptorType: Class-specific interface descriptor				20
    MIDI_CS_IF_HEADER,             // bDescriptorSubType:												31
    USBShort(0x0100),              // bcdADC, version number of this class spec							32
    USBShort(0x0061),              // wTotalLength, size of this descriptor								34

    // Jack 1
    USB_MIDI_IN_JACK_DESC_SIZE,    // bLength, size of a MIDI IN Jack descriptor						36
    USB_DTYPE_CS_INTERFACE,        // bDescriptorType, this is a class-specific interface descriptor	37
    MIDI_CS_IF_IN_JACK,            // bDescriptorSubType, a MIDI In Jack descriptor						38
    MIDI_JACKTYPE_Embedded,        // bJackType, embedded												39
    JACK1_EMBIN,                   // bJackID, this is Jack 1											40
    0,                             // iJack, no string.													41

    // Jack 2
    USB_MIDI_OUT_JACK_DESCRIPTOR_SIZE(1),  // bLength, size of a MIDI OUT Jack descriptor				42
    USB_DTYPE_CS_INTERFACE,        // bDescriptorType, this is a class-specific interface descriptor	43
    MIDI_CS_IF_OUT_JACK,           // bDescriptorSubType, a MIDI Out Jack descriptor					44
    MIDI_JACKTYPE_External,        // bJackType, external												45
    JACK2_EXTOUT,                  // bJackID, this is Jack 2											46
    1,                             // bNrInputPins, just one											47
    JACK1_EMBIN,                   // baSourceID, input source is Jack 1 only							48
    1,                             // baSourcePin, jack 1 is on input pin 1								49
    0,                             // iJack, no string													50

	// Jack 3
	USB_MIDI_IN_JACK_DESC_SIZE,    // bLength, size of a MIDI IN Jack descriptor						51
	USB_DTYPE_CS_INTERFACE,        // bDescriptorType, this is a class-specific interface descriptor	52
	MIDI_CS_IF_IN_JACK,            // bDescriptorSubType, a MIDI In Jack descriptor						53
	MIDI_JACKTYPE_External,        // bJackType, external												54
	JACK3_EXTIN,                   // bJackID, this is Jack 3											55
	0,                             // iJack, no string.													56

	// Jack 4
	USB_MIDI_OUT_JACK_DESCRIPTOR_SIZE(1),  // bLength, size of a MIDI OUT Jack descriptor				57
	USB_DTYPE_CS_INTERFACE,        // bDescriptorType, this is a class-specific interface descriptor	58
	MIDI_CS_IF_OUT_JACK,           // bDescriptorSubType, a MIDI Out Jack descriptor					59
	MIDI_JACKTYPE_Embedded,        // bJackType, embedded												60
	JACK4_EMBOUT,                  // bJackID, this is Jack 4											61
	1,                             // bNrInputPins, just one											62
	JACK3_EXTIN,                   // baSourceID, input source is Jack 3 only							63
	1,                             // baSourcePin, jack 3 is on input pin 1								64
	0,                             // iJack, no string													65

	// Jack 5
	USB_MIDI_IN_JACK_DESC_SIZE,    // bLength, size of a MIDI IN Jack descriptor						66
	USB_DTYPE_CS_INTERFACE,        // bDescriptorType, this is a class-specific interface descriptor	67
	MIDI_CS_IF_IN_JACK,            // bDescriptorSubType, a MIDI In Jack descriptor						68
	MIDI_JACKTYPE_Embedded,        // bJackType, embedded												69
	JACK5_EMBIN,                   // bJackID, this is Jack 5											70
	0,                             // iJack, no string.													71

	// Jack 6
	USB_MIDI_OUT_JACK_DESCRIPTOR_SIZE(1),  // bLength, size of a MIDI OUT Jack descriptor				72
	USB_DTYPE_CS_INTERFACE,        // bDescriptorType, this is a class-specific interface descriptor	73
	MIDI_CS_IF_OUT_JACK,           // bDescriptorSubType, a MIDI Out Jack descriptor					74
	MIDI_JACKTYPE_External,        // bJackType, external												75
	JACK6_EXTOUT,                  // bJackID, this is Jack 6											76
	1,                             // bNrInputPins, just one											77
	JACK5_EMBIN,                   // baSourceID, input source is Jack 5 only							78
	1,                             // baSourcePin, jack 5 is on input pin 1								79
	0,                             // iJack, no string													80

	// Jack 7
	USB_MIDI_IN_JACK_DESC_SIZE,    // bLength, size of a MIDI IN Jack descriptor						81
	USB_DTYPE_CS_INTERFACE,        // bDescriptorType, this is a class-specific interface descriptor	82
	MIDI_CS_IF_IN_JACK,            // bDescriptorSubType, a MIDI In Jack descriptor						83
	MIDI_JACKTYPE_External,        // bJackType, external												84
	JACK7_EXTIN,                   // bJackID, this is Jack 7											85
	0,                             // iJack, no string.													86

	// Jack 8
	USB_MIDI_OUT_JACK_DESCRIPTOR_SIZE(1),  // bLength, size of a MIDI OUT Jack descriptor				87
	USB_DTYPE_CS_INTERFACE,        // bDescriptorType, this is a class-specific interface descriptor	88
	MIDI_CS_IF_OUT_JACK,           // bDescriptorSubType, a MIDI Out Jack descriptor					89
	MIDI_JACKTYPE_Embedded,        // bJackType, embedded												90
	JACK8_EMBOUT,                  // bJackID, this is Jack 8											91
	1,                             // bNrInputPins, just one											92
	JACK7_EXTIN,                   // baSourceID, input source is Jack 7 only							93
	1,                             // baSourcePin, jack 3 is on input pin 1								94
	0,                             // iJack, no string													95

	// In Endpoint 1 Standard descriptor
	7,                            // bLength, endpoint descriptors are 7 bytes							96
	USB_DTYPE_ENDPOINT,           // bDescriptorSubType, this is a standard endpoint descriptor			97
	USB_EP_DESC_IN | USBMIDI_MS_EP_IN,  // bEndpointAddress is IN Endpoint 1							98
	USB_EP_ATTR_BULK,             // bmAttributes, this is a bulk endpoint								99
	USBShort(64),                 // wMaxPacketSize, 64 is max for full speed bulk endpoint				100
	0,                            // bInterval, must be 0 for bulk endpoint								102

	// Class-specific IN Endpoint descriptor
	USB_MIDI_CS_STREAMING_BULK_ENDPOINT_SIZE(2),  // bLength											103
	USB_CS_ENDPOINT_DESCRIPTOR,       // bDescriptorType												104
	USB_MIDI_CS_EP_MS_GENERAL,        // bDescriptorSubType												105
	2,                                // bNumEmbMIDIJack, how many embedded jacks in this direction?	106
	JACK4_EMBOUT,                     // baAssocJackID 1												107
	JACK8_EMBOUT,                     // baAssocJackID 2												108

	// Out Endpoint 1 standard descriptor
	7,                               // bLength															109
	USB_DTYPE_ENDPOINT,              // bDescriptorType, it's an endpoint								110
	USB_EP_DESC_OUT | USBMIDI_MS_EP_OUT,    // bEndpointAddress, OUT EP 1								111
	USB_EP_ATTR_BULK,                // bmAttributes, bulk endpoint										112
	USBShort(64),                    // wMaxPacketSize, 64 is max for full speed bulk endpoint			113
	0,                              // bInterval, must be 0 for bulk endpoint							115

	// Class-specific OUT endpoint descriptor
	USB_MIDI_CS_STREAMING_BULK_ENDPOINT_SIZE(2),  // bLength											116
	USB_CS_ENDPOINT_DESCRIPTOR,       // bDescriptorType												117
	USB_MIDI_CS_EP_MS_GENERAL,        // bDescriptorSubType												118
	2,                                // bNumEmbMIDIJack, how many embedded jacks in this direction?	119
	JACK1_EMBIN,                     // baAssocJackID 1													120
	JACK5_EMBIN                      // baAssocJackID 2													121
};

/**
 * The MIDI device configuration descriptor is defined as three sections.
 * One contains the 9 byte USB configuration descriptor.
 */
const tConfigSection g_sMidiConfigSection =
{
	.ui16Size = sizeof(g_pui8MidiDescriptor),
	.pui8Data = g_pui8MidiDescriptor
};

/**
 * The second holds the MIDI Streaming interface.
 */
const tConfigSection g_sMidiStreamInterfaceSection =
{
	.ui16Size = sizeof(g_pui8MidiStreamInterface),
	.pui8Data = g_pui8MidiStreamInterface
};

/**
 * the third holds the audio control interface.
 */
const tConfigSection g_sAudioMidiControlInterfaceSection =
{
	.ui16Size = sizeof(g_pui8AudioMidiControlInterface),
	.pui8Data = g_pui8AudioMidiControlInterface
};

/**
 * c.
 * Concatenate all of the sections into one array.
 */
const tConfigSection *g_psMidiSections[] =
{
	&g_sMidiConfigSection,
//	&g_sIADMidiConfigSection,
	&g_sAudioMidiControlInterfaceSection,
	&g_sMidiStreamInterfaceSection
};

#define NUM_MIDI_SECTIONS (sizeof(g_psMidiSections) / sizeof(g_psMidiSections[0]))

/**
 * d.
 * Put that concatenated section into a header.
 */
const tConfigHeader g_sMidiConfigHeader =
{
	.ui8NumSections = NUM_MIDI_SECTIONS,
 	.psSections     = g_psMidiSections
};

/**
 * e.
 * Make an array of headers (one element) for the entire configuration descriptor.
 */
const tConfigHeader * const g_ppMidiConfigDescriptors[] =
{
	&g_sMidiConfigHeader
};

/**
 * String descriptors.
 */

/* Language supported */
const uint8_t g_pui8LangDescriptor[] =
{
    4,
    USB_DTYPE_STRING,
    USBShort(USB_LANG_EN_US)
};

/*
 * The manufacturer string.
 */
const uint8_t g_pui8ManufacturerString[] =
{
    (11 + 1) * 2,
    USB_DTYPE_STRING,
    'A', 0, 'S', 0, 'P', 0, ' ', 0, 'D', 0, 'i', 0, 'g', 0, 'i', 0, 't', 0,
    'a', 0, 'l', 0
};

/*
 * The product string.
 */
const uint8_t g_pui8ProductString[] =
{
    (13 + 1) * 2,
    USB_DTYPE_STRING,
    'T', 0, 'M', 0, '4', 0, 'C', 0, ' ', 0, 'U', 0, 'S', 0, 'B', 0, '-', 0,
    'M', 0, 'I', 0, 'D', 0, 'I', 0
};

/*
 * The serial number string.
 */
const uint8_t g_pui8SerialNumberString[] =
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0
};

/**
 * The configuration description string.
 */
const uint8_t g_pui8ConfigString[] =
{
    2 + (26 * 2),
    USB_DTYPE_STRING,
    'S', 0, 'e', 0, 'l', 0, 'f', 0, ' ', 0, 'P', 0, 'o', 0, 'w', 0,
    'e', 0, 'r', 0, 'e', 0, 'd', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0,
    'f', 0, 'i', 0, 'g', 0, 'u', 0, 'r', 0, 'a', 0, 't', 0, 'i', 0,
    'o', 0, 'n', 0
};

/**
 * The descriptor string table.
 */
const uint8_t *const stringDescriptors[] =
{
	g_pui8LangDescriptor,
	g_pui8ManufacturerString,
	g_pui8ProductString,
	g_pui8SerialNumberString,
	g_pui8ConfigString
};

#define NUM_STRING_DESCRIPTORS (sizeof(stringDescriptors) / sizeof(uint8_t *))

/**
 * Structure containing the names of the handlers for the various
 * USB event interrupts.
 */
static const tCustomHandlers MidiHandlers =
{
	.pfnGetDescriptor     = 0,						// We only deal with standard descriptors
	.pfnRequestHandler    = HandleRequests,			// There are no EP0 requests in this class
	.pfnInterfaceChange   = 0,						// We have no alternate interfaces
	.pfnConfigChange      = HandleConfigChange,		// Check for the selected configuration, indicate connected
	.pfnDataReceived      = 0, 						// We do not handle data for EP0
	.pfnDataSent          = 0,						// We do not handle data for EP0
	.pfnResetHandler      = 0,						// We do not need to handle bus reset (now)
	.pfnSuspendHandler    = HandleSuspend,			// Handle USB suspension
	.pfnResumeHandler     = HandleResume,			// Handle USB resume
	.pfnDisconnectHandler = HandleDisconnect,		// Indicate no longer connected to bus
	.pfnEndpointHandler   = HandleEndpoints, 		// IN and OUT data end up here
	.pfnDeviceHandler     = 0						// We are not using the TI composite shit
};

/**
 * The Device Info structure we pass to the initializer USBDCDInit().
 */
static tDeviceInfo USBMIDIDeviceInfo =
{
    .psCallbacks              = &MidiHandlers,
    .pui8DeviceDescriptor     = g_pui8AudioMIDIDeviceDescriptor,
    .ppsConfigDescriptors     = g_ppMidiConfigDescriptors,
    .ppui8StringDescriptors   = stringDescriptors,
    .ui32NumStringDescriptors = NUM_STRING_DESCRIPTORS
};

/**
 * This structure holds all of the working information about this device.
 */
static tUSBMidiDevice g_sUsbMidiDevice;

/**
 * Initialize this device.
 * The various structures are all set up above.
 *
 * The third parameter, void *pvDCDCBData, is a monster of redirection sorcery.
 * Shoot the idiot who thought of this.
 * In the bulk example, that parameter is tUSBDBulkDevice *.
 * In USBDCDInit(), that structure is saved in the global g_psDCDInst.pvCBData member.
 */
void USBMIDI_Init(uint32_t index)
{
	USBMIDIFIFO_Init(&g_sUsbMidiDevice.InEpMsgFifo);
	USBMIDIFIFO_Init(&g_sUsbMidiDevice.OutEpMsgFifo);

	USBDCDInit(index, 				// index of USB hardware (not base address)
			&USBMIDIDeviceInfo, 	// tDeviceInfo
			&g_sUsbMidiDevice);		// "callback data for any device callbacks."
}

/**
 * Return true if the USB device is connected to the bus.
 */
bool USBMIDI_IsConnected(void)
{
	return g_sUsbMidiDevice.sPrivateData.bConnected;
}

/**
 * Functions to access the message FIFOs.
 *
 * Pop the OUT Endpoint FIFO, which returns messages sent to us from the host.
 * Returns true if msg holds a valid new message. Returns false if no message was available.
 */
bool USBMIDI_OutEpFIFO_Pop(USBMIDI_Message_t *msg)
{
	return USBMIDIFIFO_Pop(&g_sUsbMidiDevice.OutEpMsgFifo, msg);
}

/**
 * Write a new outgoing message back to the host over the IN endpoint, if the USB
 * device is actually connected. Otherwise, just drop the message on the floor.
 *
 * This writes the message to the outgoing (IN endpoint) FIFO.
 *
 * After pushing the byte to the FIFO, check to see if the endpoint is busy sending
 * a previous USB packet. If it is not, then "prime the pump."
 *
 */
void USBMIDI_InEpMsgWrite(USBMIDI_Message_t *msg)
{
	if( g_sUsbMidiDevice.sPrivateData.bConnected )
	{
		USBMIDIFIFO_Push(&g_sUsbMidiDevice.InEpMsgFifo, msg);
		if( g_sUsbMidiDevice.sPrivateData.iUSBMidiTxState == eUsbMidiStateIdle )
		{
			USBMIDI_InEpSendMessages();
		}
	}
}

/**
 * Check to see if transmit (IN endpoint) message FIFO has things to send.
 * If it does, repeatedly pop the FIFO and write the message bytes into
 * a buffer which then gets loaded into the endpoint buffer and transmitted.
 *
 * This is called either by USBMIDI_InEpMsgWrite() after a new message is pushed
 * onto the IN endpoint transmit FIFO, or by the EndpointHandler callback after
 * a previous USB packet has finished transmitting.
 *
 * USB Packet size is 64 bytes and there are 4 bytes per message, so we can
 * put a maximum of 16 messages in one packet. I will eventually change from
 * the magic number 16 to a constant.
 */
void USBMIDI_InEpSendMessages(void)
{
	uint8_t buf[64];
	uint8_t *pbuf;
	uint8_t msgByteCnt = 0;
	USBMIDI_Message_t msg;

	pbuf = buf;

	// As long as we have messages to send, pop them
	while( USBMIDIFIFO_Pop(&g_sUsbMidiDevice.InEpMsgFifo, &msg) && (msgByteCnt < 64) )
	{
		msgByteCnt += 4;
		*pbuf++ = msg.header;
		*pbuf++ = msg.byte1;
		*pbuf++ = msg.byte2;
		*pbuf++ = msg.byte3;
	}

	// Load up the endpoint FIFO!
	// Since this is called only when we know the endpoint FIFO is ready to
	// accept a new packet.
	if( msgByteCnt )
	{
		g_sUsbMidiDevice.sPrivateData.iUSBMidiTxState == eUsbMidiStateWaitData;
		USBEndpointDataPut(USB0_BASE, USB_EP_1, buf, msgByteCnt);
		USBEndpointDataSend(USB0_BASE, USB_EP_1, USB_TRANS_IN );
	}
}
