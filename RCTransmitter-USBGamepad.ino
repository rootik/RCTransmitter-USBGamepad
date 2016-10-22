
/**************************************************************************************************
 * This sketch turns Arduino Leonardo (or Pro Micro) into a plug-n-play DirectX compatible
 * USB Gamepad adapter for 6 channel RC Transmitter.
 * The adapter can be used to play FPV Freerider (http://fpv-freerider.itch.io/fpv-freerider),
 * aerofly RC 7 (http://www.aeroflyrc.com/)
 * or other flight simulators. 
 * 
 * Connections:
 * - RC PPM out <--> Digital Pin 4 of Arduino Leonardo (Pro Micro)
 * - RC GND  <--> Arduino GND
 * 
 * Author:    rootik <roootik@gmail.com>
 * - github:	https://github.com/rootik/RCTransmitter-USBGamepad
**************************************************************************************************/

#include "Arduino.h"
#include <avr/interrupt.h>
#include "USBGamepad.h"

// Use for Futaba transmitters (they have shifted center value and narrower range by default)
//#define FUTABA

// Use to enable output of PPM values to serial
//#define DEBUG

#define RC_CHANNELS_COUNT 6

#ifdef FUTABA
  #define MIN_PULSE_WIDTH 1080
  #define MAX_PULSE_WIDTH 1980
	#define THRESHOLD 200
#else
	#define MIN_PULSE_WIDTH 1000
	#define MAX_PULSE_WIDTH 2000
	#define THRESHOLD 100 // threshold is used to detect PPM values (added to range at both ends)
#endif

#define NEWFRAME_PULSE_WIDTH 3000

// timer capture ICP1 pin corresponds to Leonardo digital pin 4
#define PPM_CAPTURE_PIN 4

// for timer prescaler set to 1/8 of 16MHz, counter values should be
//  divided by 2 to get the number of microseconds
#define TIMER_COUNT_DIVIDER 2 

volatile boolean triggered;
volatile uint16_t counter;
uint16_t pulse;
uint16_t last;
unsigned long timer; // the timer
uint16_t INTERVAL = 1000; // the repeat interval

// this array contains the lengths of read PPM pulses in microseconds
uint16_t rcChannels[RC_CHANNELS_COUNT];
uint8_t chan = 0; // current channel

void setup() {
  pinMode(PPM_CAPTURE_PIN, INPUT_PULLUP);

  // Input Capture setup
  // ICNC1: =0 Disable Input Capture Noise Canceler to prevent delay in reading
  // ICES1: =1 for trigger on rising edge
  // CS11: =1 set prescaler to 1/8 system clock (F_CPU)
  TCCR1A = 0;
  TCCR1B = (0<<ICNC1) | (1<<ICES1) | (1<<CS11);
  TCCR1C = 0;

  // Interrupt setup
  // ICIE1: Input capture 
  TIFR1 = (1<<ICF1); // clear pending
  TIMSK1 = (1<<ICIE1); // and enable
  
	USBGamepad.begin(rcChannels);  

#ifdef DEBUG
  timer = millis(); // start timer
	Serial.begin(115200);
#endif

}

void loop(){
  if (!triggered)
    return;
  pulse = counter - last;
  last = counter;
  if(pulse > (NEWFRAME_PULSE_WIDTH * TIMER_COUNT_DIVIDER)) {
    chan = 0;  // new data frame detected, start again
    USBGamepad.write(rcChannels);
  }
  else {
    if(pulse > (MIN_PULSE_WIDTH * TIMER_COUNT_DIVIDER - THRESHOLD) 
      && pulse < (MAX_PULSE_WIDTH * TIMER_COUNT_DIVIDER + THRESHOLD) 
      && chan < RC_CHANNELS_COUNT) 
    {
      rcChannels[chan] = pulse / TIMER_COUNT_DIVIDER; //store detected value      
    }
    chan++; //no value detected within expected range, move to next channel
  }
  
  #ifdef DEBUG
  if ((millis() - timer) > INTERVAL && chan == 0) {
      // timed out
      timer += INTERVAL; // reset timer by moving it along to the next interval 
  	Serial.print(rcChannels[0]); 
  	Serial.print("\t");
  	Serial.print(rcChannels[1]); 
  	Serial.print("\t");
  	Serial.print(rcChannels[2]); 
  	Serial.print("\t");
  	Serial.print(rcChannels[3]); 
  	Serial.print("\t");
  	Serial.print(rcChannels[4]); 
  	Serial.print("\t");
  	Serial.println(rcChannels[5]); 	
  }
  #endif
  
  triggered = false;
}

ISR(TIMER1_CAPT_vect) {
  counter = ICR1;
  triggered = true;
}
