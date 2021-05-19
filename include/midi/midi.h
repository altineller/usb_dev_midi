/*
 * midi.h
 *
 *  Created on: Apr 20, 2019
 *      Author: andy
 *
 * MIDI functions and definitions.
 *
 * This module abstracts access to MIDI events, meaning it doesn't assume that the transport
 * is UART or USB.
 *
 * Mod:
 * 2019-08-07 asp: Add definitions of the control change messages.
 * 2020-01-20 asp: Copy to Tiva-M4C design. Add usb-midi constant definitions.
 */

#ifndef MIDI_H_
#define MIDI_H_

#include <stdint.h>

/**
 * \typedef MIDI_Message_t
 * MIDI Message Packets.
 */
#if 0
typedef struct
{
	uint8_t byte1;		// MIDI_0
	uint8_t byte2;		// MIDI_1
	uint8_t byte3;		// MIDI_2
} MIDI_Message_t;
#endif

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
#if 0
typedef union
{
    struct
    {
        uint8_t header;     //!< Cable Number, Code Index Number
        uint8_t byte1;      //!< MIDI_0
        uint8_t byte2;      //!< MIDI_1
        uint8_t byte3;      //!< MIDI_2
    } mep_packet;
    uint32_t packet;        //!< access as one 32-bit value
} USBMIDI_Message_t;
#endif

/*
 * Define the various MIDI message types.
 */
#define MIDI_MSG_NOTEOFF         0x80
#define MIDI_MSG_NOTEON          0x90
#define MIDI_MSG_POLYPRESSURE    0xA0
#define MIDI_MSG_CTRLCHANGE      0xB0
#define MIDI_MSG_PROGCHANGE      0xC0
#define MIDI_MSG_CHANNELPRESSURE 0xD0
#define MIDI_MSG_PITCHBEND       0xE0
#define MIDI_MSG_SOX             0xF0
#define MIDI_MSG_MTCQF 			 0xF1
#define MIDI_MSG_SPP             0xF2
#define MIDI_MSG_SS              0xF3
#define MIDI_MSG_F4              0xF4
#define MIDI_MSG_F5              0xF5
#define MIDI_MSG_TUNEREQ         0xF6
#define MIDI_MSG_EOX             0xF7
#define MIDI_MSG_TIMINGCLOCK     0xF8
#define MIDI_MSG_F9              0xF9
#define MIDI_MSG_START           0xFA
#define MIDI_MSG_CONTINUE        0xFB
#define MIDI_MSG_STOP            0xFC
#define MIDI_MSG_FD              0xFD
#define MIDI_MSG_ACTIVESENSING   0xFE
#define MIDI_MSG_SYSRST          0xFF

/**
 * Construct a MIDI STATUS byte from a given MIDI message and channel.
 */
#define MIDI_STATUS_BYTE(message, channel)  (message | channel)

/**
 * These are the defined MIDI control-change messages (DATA bytes)
 */
#define MIDI_CC_BANKSELECT_MSB		0x00
#define MIDI_CC_MODWHEEL_MSB		0x01
#define MIDI_CC_BREATHCTRL_MSB		0x02
#define MIDI_CC_UNDEFINED03_MSB		0x03
#define MIDI_CC_FOOTCTRL_MSB		0x04
#define MIDI_CC_PORTAMENTOTIME_MSB	0x05
#define MIDI_CC_DATAENTRY_MSB		0x06
#define MIDI_CC_CHANNELVOLUME_MSB	0x07
#define MIDI_CC_BALANCE_MSB			0x08
#define MIDI_CC_UNDEFINED09_MSB		0x09
#define MIDI_CC_PAN_MSB				0x0A
#define MIDI_CC_EXPRESSION_MSB		0x0B
#define MIDI_CC_EFFECTCTRL1_MSB		0x0C
#define MIDI_CC_EFFECTCTRL2_MSB		0x0D
#define MIDI_CC_UNDEFINED0E_MSB		0x0E
#define MIDI_CC_UNDEFINED0F_MSB		0x0F
#define MIDI_CC_GP1_MSB				0x10
#define MIDI_CC_GP2_MSB				0x11
#define MIDI_CC_GP3_MSB				0x12
#define MIDI_CC_GP4_MSB				0x13
#define MIDI_CC_UNDEFINED14_MSB		0x14
#define MIDI_CC_UNDEFINED15_MSB		0x15
#define MIDI_CC_UNDEFINED16_MSB		0x16
#define MIDI_CC_UNDEFINED17_MSB		0x17
#define MIDI_CC_UNDEFINED18_MSB		0x18
#define MIDI_CC_UNDEFINED19_MSB		0x19
#define MIDI_CC_UNDEFINED1A_MSB		0x1A
#define MIDI_CC_UNDEFINED1B_MSB		0x1B
#define MIDI_CC_UNDEFINED1C_MSB		0x1C
#define MIDI_CC_UNDEFINED1D_MSB		0x1D
#define MIDI_CC_UNDEFINED1E_MSB		0x1E
#define MIDI_CC_UNDEFINED1F_MSB		0x1F
#define MIDI_CC_BANKSELECT_LSB		0x20
#define MIDI_CC_MODWHEEL_LSB		0x21
#define MIDI_CC_BREATHCTRL_LSB		0x22
#define MIDI_CC_UNDEFINED03_LSB		0x23
#define MIDI_CC_FOOTCTRL_LSB		0x24
#define MIDI_CC_PORTAMENTOTIME_LSB	0x25
#define MIDI_CC_DATAENTRY_LSB		0x26
#define MIDI_CC_CHANNELVOLUME_LSB	0x27
#define MIDI_CC_BALANCE_LSB			0x28
#define MIDI_CC_UNDEFINED09_LSB		0x29
#define MIDI_CC_PAN_LSB				0x2A
#define MIDI_CC_EXPRESSION_LSB		0x2B
#define MIDI_CC_EFFECTCTRL1_LSB		0x2C
#define MIDI_CC_EFFECTCTRL2_LSB		0x2D
#define MIDI_CC_UNDEFINED0E_LSB		0x2E
#define MIDI_CC_UNDEFINED0F_LSB		0x2F
#define MIDI_CC_GP1_LSB				0x30
#define MIDI_CC_GP2_LSB				0x31
#define MIDI_CC_GP3_LSB				0x32
#define MIDI_CC_GP4_LSB				0x33
#define MIDI_CC_UNDEFINED14_LSB		0x34
#define MIDI_CC_UNDEFINED15_LSB		0x35
#define MIDI_CC_UNDEFINED16_LSB		0x36
#define MIDI_CC_UNDEFINED17_LSB		0x37
#define MIDI_CC_UNDEFINED18_LSB		0x38
#define MIDI_CC_UNDEFINED19_LSB		0x39
#define MIDI_CC_UNDEFINED1A_LSB		0x3A
#define MIDI_CC_UNDEFINED1B_LSB		0x3B
#define MIDI_CC_UNDEFINED1C_LSB		0x3C
#define MIDI_CC_UNDEFINED1D_LSB		0x3D
#define MIDI_CC_UNDEFINED1E_LSB		0x3E
#define MIDI_CC_UNDEFINED1F_LSB		0x3F
#define MIDI_CC_DAMPERONOFF			0x40
#define MIDI_CC_PORTAMENTOONOFF		0x41
#define MIDI_CC_SOSTENUTOONOFF		0x42
#define MIDI_CC_SOFTPEDALONOFF		0x43
#define MIDI_CC_LEGATOFS			0x44
#define MIDI_CC_HOLD2ONOFF			0x45
#define MIDI_CC_SOUNDCONTROLLER1	0x46
#define MIDI_CC_SOUNDCONTROLLER2	0x47
#define MIDI_CC_SOUNDCONTROLLER3	0x48
#define MIDI_CC_SOUNDCONTROLLER4	0x49
#define MIDI_CC_SOUNDCONTROLLER5	0x4A
#define MIDI_CC_SOUNDCONTROLLER6	0x4B
#define MIDI_CC_SOUNDCONTROLLER7	0x4C
#define MIDI_CC_SOUNDCONTROLLER8	0x4D
#define MIDI_CC_SOUNDCONTROLLER9	0x4E
#define MIDI_CC_SOUNDCONTROLLER10 	0x4F
#define MIDI_CC_GP5					0x50
#define MIDI_CC_GP6					0x51
#define MIDI_CC_GP7					0x52
#define MIDI_CC_GP8					0x53
#define MIDI_CC_PORTAMENTOCTRL		0x54
#define MIDI_CC_UNDEFINED55			0x55
#define MIDI_CC_UNDEFINED56			0x56
#define MIDI_CC_UNDEFINED57			0x57
#define MIDI_CC_HIRESVELOCITYPREFIX 0x58
#define MIDI_CC_UNDEFINED59			0x59
#define MIDI_CC_UNDEFINED5A			0x5A
#define MIDI_CC_EFFECTS1DEPTH		0x5B
#define MIDI_CC_EFFECTS2DEPTH		0x5C
#define MIDI_CC_EFFECTS3DEPTH		0x5D
#define MIDI_CC_EFFECTS4DEPTH		0x5E
#define MIDI_CC_EFFECTS5DEPTH		0x5F
#define MIDI_CC_DATAINCREMENT		0x60
#define MIDI_CC_DATADECREMENT		0x61
#define MIDI_CC_NRPN_LSB			0x62
#define MIDI_CC_NRPN_MSB			0x63
#define MIDI_CC_RPN_LSB				0x64
#define MIDI_CC_RPN_MSB				0x65
#define MIDI_CC_UNDEFINED66			0x66
#define MIDI_CC_UNDEFINED67			0x67
#define MIDI_CC_UNDEFINED68			0x68
#define MIDI_CC_UNDEFINED69			0x69
#define MIDI_CC_UNDEFINED6A			0x6A
#define MIDI_CC_UNDEFINED6B			0x6B
#define MIDI_CC_UNDEFINED6C			0x6C
#define MIDI_CC_UNDEFINED6D			0x6D
#define MIDI_CC_UNDEFINED6E			0x6E
#define MIDI_CC_UNDEFINED6F			0x6F
#define MIDI_CC_UNDEFINED70			0x70
#define MIDI_CC_UNDEFINED71			0x71
#define MIDI_CC_UNDEFINED72			0x72
#define MIDI_CC_UNDEFINED73			0x73
#define MIDI_CC_UNDEFINED74			0x74
#define MIDI_CC_UNDEFINED75			0x75
#define MIDI_CC_UNDEFINED76			0x76
#define MIDI_CC_UNDEFINED77			0x77
#define MIDI_CC_CMM_ALLSOUNDOFF		0x78
#define MIDI_CC_CMM_RESETALLCTRLS	0x79
#define MIDI_CC_CMM_LOCALCTRLONOFF	0x7A
#define MIDI_CC_CMM_ALLNOTESOFF		0x7B
#define MIDI_CC_CMM_OMNIMODEOFF		0x7C
#define MIDI_CC_CMM_OMNIMODEON		0x7D
#define MIDI_CC_CMM_MONOMODEON		0x7E
#define MIDI_CC_CMM_POLYMODEON		0x7F

/**
 * MIDI Code Index Number Classifications.
 * Please refer to the USB MIDI Device Class Spec, table 4-1.
 * This is the lower nibble of the header (byte 0) of the USB-MIDI Event Packet)
 */

/*! \def Reserve for future expansion */
#define USB_MIDI_CIN_MISC           0x0
/*! \def Cable Events, reserved for future expansion */
#define USB_MIDI_CIN_CABLEEVENTS    0x1
/*! \def Two-byte System Common messages */
#define USB_MIDI_CIN_SYSCOM2        0x2
/*! \def Three-bytes System Common messages. */
#define USB_MIDI_CIN_SYSCOM3        0x3
/*! \def SysEx starts or continues */
#define USB_MIDI_CIN_SYSEXSTART     0x4
/*! \def Single-byte System Common or SysEx ends */
#define USB_MIDI_CIN_SYSEND1        0x5
/*! \def SysEx ends with following two bytes */
#define USB_MIDI_CIN_SYSEND2        0x6
/*! \def SysEx ends with following three bytes */
#define USB_MIDI_CIN_SYSEND3        0x7
/*! \def Note Off */
#define USB_MIDI_CIN_NOTEOFF        0x8
/*! \def Note On */
#define USB_MIDI_CIN_NOTEON         0x9
/*! \def Poly KeyPress */
#define USB_MIDI_CIN_POLYKEYPRESS   0xA
/*! \def Control change */
#define USB_MIDI_CIN_CTRLCHANGE     0xB
/*! \def Program change */
#define USB_MIDI_CIN_PROGCHANGE     0xC
/*! \def Channel Pressure */
#define USB_MIDI_CIN_CHANPRESSURE   0xD
/*! \def Pitchbend change */
#define USB_MIDI_CIN_PITCHBEND      0xE
/*! \def Single Byte */
#define USB_MIDI_CIN_SINGLEBYTE     0xF

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

#endif /* MIDI_H_ */
