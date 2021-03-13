# ArduinoAbletonLed
Code for controlling LEDs with Arduino from Ableton (via Max for Live - Cycling 74)

# Installation
1) Copy file in the "MaxForLive_code" directory to Documents\Ableton\User Library\Presets\MIDI Effects\Max MIDI Effect
2) Copy folder in the "Arduino_code" directory to Documents\Arduino
3) Open Arduino IDE and configure the configure output ports where LEDs are connected (LED_1 through LED_4, LED_COMS and LED_MEASURE)
4) Program Arduino/Teensy/ESP32 with ArduinoAbletonLed.ino (and LEAVE the device connected to the computer via USB)
5) Open Ableton and on the left goto Catagories -> Max for Live -> Max MIDI Effect and move ArduinoAbletonLed to a midi track

# How to use
Load ArduinoAbletonLed MIDI effect in a MIDI channel in Ableton. Make sure the Arduino is connected through the serial port and that you are not using the serial port with another program (such as the Arduino IDE COM monitor for instance). Press "Rescan" to connect to Arduino. Play your track and the LEDs should work. I tried to add as much as explanation as possible, so should be self-explanatory, but will also add a Youtube video for further explanations.

Have fun ;-)

mo thunderz
