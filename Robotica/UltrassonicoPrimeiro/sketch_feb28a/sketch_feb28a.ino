#include "Ultrasonic.h" //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO
#include <string.h>
 
#define echoPin 7 //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
#define trigPin 6 //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)
#define verde 5
#define amarelo 4
#define vermelho 3
#define bichao 8
 
Ultrasonic ultrasonic(trigPin,echoPin); //INICIALIZANDO OS PINOS DO ARDUINO
 
int distancia; //VARIÁVEL DO TIPO INTEIRO
String result; //VARIÁVEL DO TIPO STRING
String cor;

float meioSeg = 57723.5;
float umSeg = 49911;
float doisSeg = 34286;
 
void setup(){
  pinMode(echoPin, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)
  pinMode(verde, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(vermelho, OUTPUT);
  pinMode(bichao, OUTPUT);
  digitalWrite(bichao,1);
  Serial.begin(9600); //INICIALIZA A PORTA SERIAL
 
  noInterrupts();                       // disable all interrupts 
  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1 = umSeg;                        // preload timer 
  TCCR1B |= (1 << CS10)|(1 << CS12);    // 1024 prescaler 
  TIMSK1 |= (1 << TOIE1);               // enable timer overflow interrupt ISR 
  interrupts();                         // enable all interrupts 
  }
void loop(){
  
  hcsr04(); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  Serial.print("Distancia "); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(result); ////IMPRIME NO MONITOR SERIAL A DISTÂNCIA MEDIDA
  Serial.println("cm"); //IMPRIME O TEXTO NO MONITOR SERIAL

  
}
//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA
void hcsr04(){
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
    delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
    digitalWrite(trigPin, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
    delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
    //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
    //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
    //NA VARIAVEL "distancia"
    distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
    result = String(distancia); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
    if(distancia < 10){
      if(cor != "vermei"){
        cor = "vermei";
        digitalWrite(verde, 0);
        digitalWrite(amarelo, 0);
        TCNT1 = 65530;
      }
      return;
    }
    if(distancia < 20){
      if(cor != "marelo"){
        cor = "marelo";
        digitalWrite(verde, 0);
        digitalWrite(vermelho, 0);
        digitalWrite(bichao,1);
        TCNT1 = 65530;
      }
      return;
    }
    if(cor != "verde"){
      cor = "verde";
      digitalWrite(amarelo, 0);
      digitalWrite(vermelho, 0);
      digitalWrite(bichao,1);
      TCNT1 = 65530;
    }
    delay(20);
 }

 ISR(TIMER1_OVF_vect){                    // interrupt service routine for overflow
  if(cor == "vermei"){
    TCNT1 = meioSeg;
    digitalWrite(bichao, digitalRead(bichao) ^0b00000001);
    digitalWrite(vermelho, digitalRead(vermelho) ^0b00000001);
    return;
  }
  if(cor == "marelo"){
    TCNT1 = umSeg;
    digitalWrite(amarelo, digitalRead(amarelo) ^0b00000001);
    return; 
  }
  TCNT1 = doisSeg;
  digitalWrite(verde, digitalRead(verde) ^0b00000001);
  return;
} 