#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdio.h> 
#include <util/delay.h> 

#define F_CPU 16000000L // 16 MHz 

#define ledPin 8 
#define button 9 

float value = 1; //0xC2F8; // decimal value is 49912; 
int ledState = HIGH; //The current state of the output pin. 
int buttonState; //The current reading from the input pin. 
int lastButtonState = LOW; // The previous reading from the input pin. 

unsigned long lastDebounceTime = 0; //The last time the output pin was toggled. 
unsigned long debounceDelay = 50; //The debounce time; increase if the output flickers 

void setup(){ 
  DDRB = 0b00000001; //0X01; //set portB OUT 

  noInterrupts(); // disable all interrupts 

  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1 = value; // preload timer 
  TCCR1B |= (1 << CS10)|(1 << CS12); // 1024 prescaler  
  TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt ISR 
  
  interrupts(); // enable all interrupts 
} 

void loop(){ 
  int reading = digitalRead(button); 

  if (reading != lastButtonState){ 
    lastDebounceTime = millis(); //Reset the debouncing timer 
  } 

  if ((millis() - lastDebounceTime) > debounceDelay){ 
  //Whatever the reading is at, it's been there for longer than the debounce 
  // delay, so take it as the actual current state: 
    if (reading != buttonState){ //If the button state has changed: 
      buttonState = reading; 
      
      if (buttonState == HIGH){ //Only toggle the LED if the new button state is high 
      //ledState = !ledState; 
      value = value + 2000; 
      }
    } 
  } 
  //digitalWrite(ledPin, ledState); //Set the LED: 
  lastButtonState = reading; //Save the reading. Next time through the loop, it'll be the lastButtonState: 
} 

ISR(TIMER1_OVF_vect){ // interrupt service routine for overflow 
  TCNT1 = value; // preload timer 
  digitalWrite(ledPin, digitalRead(ledPin) ^0b00000001); //Turns LED ON and OFF//xor - eor - toogle 
} 

 