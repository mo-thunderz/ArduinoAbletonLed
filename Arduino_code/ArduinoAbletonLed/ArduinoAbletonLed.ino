// --------------------------------------------------
//
// Arduino LED, controlled from Ableton with Max For Live. 
// Notes:
//     -> Max for Live plug-in required in Ableton
//     -> see readme on https://github.com/mo-thunderz/ArduinoAbletonLed for installation instructions
//
// Four bytes interface is used between Ableton and Arduino:
// Byte 1: 255 (sync byte)
// Byte 2: <control> - [0:255]
// Byte 3: <value MSB> - [0:255]
// Byte 4: <value LSB> - [0:255]
//
// Written by mo thunderz (last update: 13.3.2021)
//
// --------------------------------------------------

#define CONNECTED_TIMEOUT 500000 // LED_BUILTIN goes on for CONNECTED_TIMEOUT micro seconds whenever a serial command is received

//
// NOTE: all ID_xxxx variables below are CONTROL numbers for interfacing with Ableton
// NOTE2: you can change these definitions as you like, but dont forget to change the according numbers in Max for Live as well.
//
#define ID_SONG_BEAT 252      // defines current beat count
#define ID_SONG_MEASURE 251   // defines the start of a measure
#define ID_SONG_BPM 250       // defines the BPM -> note: I dont use the BPM to control the LEDs, but I do transfer the BPM from Ableton to Arduino, you might want to use it ;-)

// pin assignments
#define LED_1    44         // First LED
#define LED_2    48         // Second LED
#define LED_3    52         // Third LED
#define LED_4    41         // Fourth LED
#define LED_COMS 28         // Optional LED to indicate communication from PC to Arduino
#define LED_MEASURE 40      // LED that with every measure changes state (when Ableton is playing a track)

// variables 
float bpm = 120;            // initial value -> is overwritten as soon as you play a track in Ableton

// internal variables
int rx_state = 0;           // internal state value to read out communication from Ableton to Arduino
byte cc_sync;               // these four bytes are received from Ableton
byte cc_control;
byte cc_val1;
byte cc_val2;
unsigned long   t = 0;              // current timestamp 
unsigned long   connected_t0 = 0;   // last time communication was received -> used only for LED_COMS   

int getInt(int l_highByte, int l_lowByte) {     // simple code to join 2 bytes into an unsigned int
  return ((unsigned int)l_highByte << 8) + l_lowByte;
}

void setup() {
  delay(100);               // not strictly needed, but runs better on some Arduino boards I find

  pinMode(LED_1, OUTPUT);   // init LED ports
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_COMS, OUTPUT);
  pinMode(LED_MEASURE, OUTPUT);
  
  digitalWrite(LED_1, false); // switch ports off
  digitalWrite(LED_2, false);
  digitalWrite(LED_3, false);
  digitalWrite(LED_4, false);
  digitalWrite(LED_COMS, false);
  digitalWrite(LED_MEASURE, false);
  
  Serial.begin(57600);        // make sure this value is the same as the one used in the Ableton Plugin  
}

void loop() {
  t = micros();           // take timestamp

  //-------------------------------timeout led---------------------------------//
  if (digitalRead(LED_COMS))
    if((t - connected_t0) > CONNECTED_TIMEOUT)
      digitalWrite(LED_COMS, 0);

  //----------Check if control commands have been received from Ableton------------//
  if (Serial.available()) {
    connected_t0 = t;
    if (digitalRead(LED_COMS) == 0)
      digitalWrite(LED_COMS, 1);

    rx_state++;
    switch (rx_state) {
      case 1:                     // first byte is always 255 for sync
        cc_sync = Serial.read();
        if(cc_sync != 255) {     // reset if first is not 255 sync byte
          rx_state = 0;
        }
        break;
      case 2:                     // second is the control byte
        cc_control = Serial.read();
        break;        
      case 3:                     // third is the most significant byte of the value
        cc_val1 = Serial.read();     
        break;
      case 4:                     // fourth is the least significant byte of the value
        cc_val2 = Serial.read();
        rx_state = 0;

        // re-compile value from its two bytes (cc_val1 is the MSB and cc_val2 the LSB)
        int value = getInt(cc_val1, cc_val2);
        
        // Track specific IDs
        if (cc_control == ID_SONG_BPM) {
          bpm = ((float)value)/10;
        }
        else if (cc_control == ID_SONG_MEASURE) {
            if(value !=0) {       // when track stops measure = 0         
              digitalWrite(LED_MEASURE, !digitalRead(LED_MEASURE));
            }
        }
        else if (cc_control == ID_SONG_BEAT) {
            digitalWrite(LED_1 , false);
            digitalWrite(LED_2 , false);
            digitalWrite(LED_3 , false);
            digitalWrite(LED_4 , false);
            
            if(value == 1)  
              digitalWrite(LED_1, true);
            else if (value == 2)
              digitalWrite(LED_2, true);
            else if (value == 3)
              digitalWrite(LED_3, true);
            else if (value == 4)
              digitalWrite(LED_4, true);
        }
    }
  }
}
