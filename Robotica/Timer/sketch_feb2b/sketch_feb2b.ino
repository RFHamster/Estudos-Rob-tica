//************************************************************************/ 

//                             Blink with Timer 

//  Version             : 1.0 

//  microcontroller     : AVR ATMega328P - Protoboard 

//  Author              : Diego Nunes Molinos 

//  description         : Blink using timer 

//  data                : 13/12/2018. 

// 

//************************************************************************/ 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdio.h> 
#include <util/delay.h> 

#define F_CPU 16000000L  // 16 MHz 

#define ledPin 8 
#define btnPinMais 9
#define btnPinMenos 10

float decrescimo = 1562.5;
float teto = 49911;
float chao = 34286;

float value = teto; //0xC2F8; // decimal value is 49912; 

void setup(){ 

 DDRB = 0b00000001; //0X01;  //set portB OUT 

 noInterrupts();                       // disable all interrupts 

 TCCR1A = 0; 
 TCCR1B = 0; 
 TCNT1 = value;                        // preload timer 
 TCCR1B |= (1 << CS10)|(1 << CS12);    // 1024 prescaler 
 TIMSK1 |= (1 << TOIE1);               // enable timer overflow interrupt ISR 

 interrupts();                         // enable all interrupts 

} 

bool pressionado = false;

void loop(){ 
  if(!digitalRead(btnPinMais) && !digitalRead(btnPinMenos)){
    pressionado = false;
  }

  if(digitalRead(btnPinMais) && !pressionado){
    pressionado = true;
    if(value > chao){
      value -= decrescimo;
    }
  }

  if(digitalRead(btnPinMenos) && !pressionado){
    pressionado = true;
    if(value < teto){
      value += decrescimo;
    }
  }

  
} 


ISR(TIMER1_OVF_vect){                    // interrupt service routine for overflow 
 TCNT1 = value;                                // preload timer 
 digitalWrite(ledPin, digitalRead(ledPin) ^0b00000001); //Turns LED ON and OFF//xor - eor 
} 

 

 

 