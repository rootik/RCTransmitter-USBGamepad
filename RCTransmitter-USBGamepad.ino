
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
 * 
 * License: MIT
 *************************************************************************************************/

#include "Arduino.h"
#include <avr/interrupt.h>
#include "USBGamepad.h"

// Use for Futaba transmitters (they have shifted center value and narrower range by default)
//#define FUTABA

// Uncomment to enable output of PPM values to serial
//#define DEBUG

#define RC_CHANNELS_COUNT 6

#ifdef FUTABA
  #define MIN_PULSE_WIDTH 1080
  #define MAX_PULSE_WIDTH 1980
	#define THRESHOLD 200 // threshold is used to detect PPM values (added to range at both ends)
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

volatile boolean triggered; //interrupt flag, must be volatile
volatile uint16_t counter;  //hold Timer1 counter value, must be volatile
uint16_t pulse; //pulse width
uint16_t last;  //previous counter value
boolean first = true; //first frame flag
uint16_t debug_timer; //debug timer
#define DEBUG_INTERVAL 800 //serial output every DEBUG_INTERVAL

// this array contains the lengths of read PPM pulses in microseconds
uint16_t rcChannels[RC_CHANNELS_COUNT];
uint8_t chan = 0; // current channel

ISR(TIMER1_CAPT_vect) { //Timer1 ISR capture vector
  counter = ICR1; //grab Timer1 value
  triggered = true; //we have interrupt
}

void setup() {
  cli(); //disable global interrupts until we are ready to accept them
  pinMode(PPM_CAPTURE_PIN, INPUT_PULLUP); //set PPM capture pin with pullup enabled

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
  
	USBGamepad.begin(rcChannels);  //send empty USB report

#ifdef DEBUG
  debug_timer = millis(); // start timer
	Serial.begin(115200);
#endif
  sei(); //enable global interrupts
}

void loop(){
  if (!triggered) //wait for interrupt
    return;  
  pulse = (counter - last) / TIMER_COUNT_DIVIDER; //pulse width
  last = counter; //store
  if(pulse > (NEWFRAME_PULSE_WIDTH)) {
    chan = 0;  // new data frame detected, start again    
    if (first) { //discard first frame data because it may be incomplete 
      first = false;
      triggered = false;
      return;
    } else 
    {
      USBGamepad.write(rcChannels); //we have complete frame so send usb report with channels data
    }
  }
  else {
    if(pulse > (MIN_PULSE_WIDTH - THRESHOLD) 
      && pulse < (MAX_PULSE_WIDTH + THRESHOLD) 
      && chan < RC_CHANNELS_COUNT && !first) 
    {
      rcChannels[chan] = pulse; //store detected value
      chan++; //move to next channel    
    }
    
  }
  
  #ifdef DEBUG
  if ((millis() - debug_timer) > DEBUG_INTERVAL && chan == 5) { //debug output when we have 10-12 mS pause in interrupts, while in start pulse
      // timed out
      debug_timer += DEBUG_INTERVAL; // reset timer by moving it along to the next interval 
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
  
  triggered = false; //wait for another interrupt
}
