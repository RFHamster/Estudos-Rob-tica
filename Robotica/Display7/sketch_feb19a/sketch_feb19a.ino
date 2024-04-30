#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <stdio.h> 
#include <util/delay.h> 

#define F_CPU 16000000L  // 16 MHz 

//Definindo as portas de luz do display 7seg
#define A 6
#define B 7 
#define C 2 
#define D 1 
#define E 0 
#define F 5 
#define G 4 
#define PT 3 

//Definindo as portas de alimentação dos displays
#define seg1 8
#define seg2 9
#define min1 11
#define min2 10
#define hora1 12
#define hora2 13

//Variaveis referentes ao tempo do projeto
int segundos = 0;
int minutos = 0;
int horas = 0;

//Tempo de estouro do contador (1 segundo)
float value = 49395; 

const int H = 0, L = 1; // Anode 

//Função para desligar todos os displays
void shutdown(){
  digitalWrite(seg1, 1);
  digitalWrite(seg2, 1);
  digitalWrite(min1, 1); 
  digitalWrite(min2, 1); 
  digitalWrite(hora1, 1); 
  digitalWrite(hora2, 1); 
}

//Função para mostrar o numero (cont) no display
void changeNumber(int cont){ 
  switch (cont){
    case 0: 
    PORTD = 0b11100111; ////BAFG PT CDE 
    break; 

    case 1: 
      PORTD = 0b10000100; ////BAFG PT CDE 
      break; 

    case 2: 
      PORTD = 0b11010011; ////BAFG PT CDE 
      break; 

    case 3: 
      PORTD = 0b11010110; ////BAFG PT CDE 
      break; 

    case 4: 
      PORTD = 0b10110100; ////BAFG PT CDE 
      break; 

    case 5: 
      PORTD = 0b01110110; ////BAFG PT CDE 
      break; 

    case 6: 
      PORTD = 0b01110111; ////BAFG PT CDE 
      break; 

    case 7: 
      PORTD = 0b11000100; ////BAFG PT CDE 
      break; 

    case 8: 
      PORTD = 0b11110111; ////BAFG PT CDE 
      break; 

    case 9: 
      PORTD = 0b11110110; ////BAFG PT CDE 
      break;  

    default: 
      cont = 0; 
      break; 
  }
}

//Aumentar o tempo do projeto em 1 segundo
void incHoras(){
  segundos++;
  if(segundos>59){
    segundos = 0;
    minutos++;
    if(minutos>59){
      minutos = 0;
      horas++;
      if(horas>23){
        horas = 0;
      }
    }
  }
}

//Mostrar as horas do projeto nos displays
//Utilizando multiplexação 
//(liga um, mostra seu tempo, desliga ele e repete o processo infinitamente com cada um)
void makeHoras(){

  int s1 = segundos/10;
  int s2 = segundos - (s1*10);
  digitalWrite(seg1, 0);
  changeNumber(s1);
  delay(2);
  digitalWrite(seg1, 1);
  
  digitalWrite(seg2, 0);
  changeNumber(s2);
  delay(2);
  digitalWrite(seg2, 1);

  int m1 = minutos/10;
  int m2 = minutos - (m1*10);
  digitalWrite(min1, 0);
  changeNumber(m1);
  delay(2);
  digitalWrite(min1, 1);

  digitalWrite(min2, 0);
  changeNumber(m2);
  delay(2);
  digitalWrite(min2, 1); 

  int h1 = horas/10;
  int h2 = horas - (h1*10);
  digitalWrite(hora1, 0);
  changeNumber(h1);
  delay(2);
  digitalWrite(hora1, 1); 

  digitalWrite(hora2, 0);
  changeNumber(h2);
  delay(2);
  digitalWrite(hora2, 1);
}

//Função para setar a hora inicial do projeto
void setHoras(int s, int m, int h){
  segundos = s;
  minutos = m;
  horas = h;
}

void setup(){
  //Setar horas
  setHoras(57,58,11);
  //Setar portas dos displays (alimentação e luz) como saida
  DDRD = 0b11111111; //0XFF;  //inicializa portD como saida 
  DDRB = 0b00111111; //0XFF;  //inicializa portB como saida 

  noInterrupts();                       // disable all interrupts 
  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1 = value;                        // preload timer 
  TCCR1B |= (1 << CS10)|(1 << CS12);    // 1024 prescaler 
  TIMSK1 |= (1 << TOIE1);               // enable timer overflow interrupt ISR 
  interrupts();                         // enable all interrupts 
  
} 

void loop(){ 
  shutdown();
  makeHoras();
} 

ISR(TIMER1_OVF_vect){                    // interrupt service routine for overflow 
  TCNT1 = value;
  incHoras();
} 

 

 

 

 

 