/*	Author: sspon001
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Final Project
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 */
#define F_CPU 16000000UL
#include <avr/io.h>	
#include <avr/interrupt.h>
#include "../header/io.h"

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
// adc_to_pwm.pde
// ADC to PWM converter
// guest - openmusiclabs.com - 1.9.13
// options table at http://wiki.openmusiclabs.com/wiki/PWMDAC
// takes in audio data from the ADC and plays it out on
// Timer1 PWM.  16b, Phase Correct, 31.25kHz - although ADC is 10b.

#define PWM_FREQ 0x00FF // pwm frequency - see table
#define PWM_MODE 1 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // number of pwms, either 1 or 2

void setup() {
  // setup ADC
  ADMUX = 0x60; // left adjust, adc0, internal vcc
  ADCSRA = 0xe5; // turn on adc, ck/32, auto trigger
  ADCSRB =0x07; // t1 capture for trigger
  DIDR0 = 0x01; // turn off digital inputs for adc0
  
  // setup PWM
  TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); // 
  TCCR1B = ((PWM_MODE << 3) | 0x11); // ck/1
  TIMSK1 = 0x20; // interrupt on capture interrupt
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRD |= 0x30; // turn on outputs
  sei() ;
  
}

int input, vol_variable = 512 ;
int counter =  0 ;
unsigned char ADC_low, ADC_high ;

int main() {
  setup() ;
  while(1); // gets rid of jitter
  return 1 ;
}

ISR(TIMER1_CAPT_vect) {
  // get ADC data
  ADC_low = ADCL; // you need to fetch the low byte first
  ADC_high = ADCH;
  //construct the input sumple summing the ADC low and high byte.
  input = ((ADC_high << 8) | ADC_low) + 0x8000; // make a signed 16b value
 
 
  //write the PWM signal
  OCR1AL = (input + 0x8000) >> 8 ;
  OCR1BL = input + 0x8000 ;
}

