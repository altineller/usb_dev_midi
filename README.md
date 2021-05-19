### USB MIDI for TM4C123GH6PM

Dependencies: TivaWare C Series, 2.2.0.295  

Put TivaWare under `/home/user/ti/TivaWare_C_Series-2.2.0.295` or `C:\ti\TivaWare_C_Series-2.2.0.295`  

After importing the project to your CCS workspace, right click on the project, select properties, and find the 
Resource - Linked Resources.  

Modify SW_ROOT to point the your TivaWare folder, and click apply and close.

Click Build, and it should build with no problems.

Notice: make sure the `Debug` folder is empty, while transferring code between different computers.  

#### Testing under Linux

After plugging the USB to your host computer, issue the following command:

`aseqdump -l`

It should return:

```
 Port    Client name                      Port name
  0:0    System                           Timer
  0:1    System                           Announce
 14:0    Midi Through                     Midi Through Port-0
 24:0    TM4C USB-MIDI                    TM4C USB-MIDI MIDI 1
 24:1    TM4C USB-MIDI                    TM4C USB-MIDI MIDI 2
```

24:0 is the Audio Control Interface, Input
24:1 is the Midi Streaming Interface, Output

##### Transmitting MIDI

The default program should send a note to channel 1, repeatedly, by making a 0.3s pause.  

##### Receiving MIDI

Connect your serial terminal emulator to `/dev/ttyACM0`.  

Notice: The serial terminal is accessible by LaunchPad programming connector. So you have to connect both USB cables to your launchpad.  

Issue the following command:  

`aplaymidi -p 24:0 file.mid`

On your terminal, you should see the midi notes.  

#### Echo MIDI

Comment out the `MIDI_USB_Rx_Task();` and all the other noteOn, noteOff statements from the main loop and uncomment the:  

`// MIDI_USB_Echo_Task();` statement.

Connect 24:1 to Qsynth. All the notes you send to the midi device will be echoed back to Qsynth.

#### Credits

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

- Copyright 2021 Stephan Bourgeois
- Ported to TM4C123 by Can Altineller
- Originally written by Andy Peters



