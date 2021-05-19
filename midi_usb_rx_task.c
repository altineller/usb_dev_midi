#include "midi_usb_rx_task.h"

void MIDI_USB_Rx_Task(void) {
	while(USBMIDI_OutEpFIFO_Pop(&rxmsg)) {
		UARTprintf("%02x : %02x : %02x : %02x\n", rxmsg.header, rxmsg.byte1, rxmsg.byte2, rxmsg.byte3);
	}
}

void MIDI_USB_Loop_Task(void) {
    while(USBMIDI_OutEpFIFO_Pop(&rxmsg)) {
        if(rxmsg.header!=0) {
            USBMIDI_InEpMsgWrite(&rxmsg);
            MAP_SysCtlDelay(10000);
        }
    }
}
