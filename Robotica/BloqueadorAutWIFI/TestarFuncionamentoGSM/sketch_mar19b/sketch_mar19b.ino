#include <Sim800L.h>
#include <SoftwareSerial.h>
Sim800L Sim800L(D10,D11);
String textSms, numberSms;
uint8_t LED1 = D4; //Conexao Led vermelho
uint8_t LED2 = D5; //Conexao Led verde
bool error;
void setup()
{
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Sim800L.begin();
  Serial.print("Limpando SMS antigos...");
  error = Sim800L.delAllSms(); //Apaga SMS
  Serial.println(" Apagados!");
  Serial.println("\nAguardando comandos por SMS...");
}
void loop()
{
  //Le o SMS da posicao 1
  textSms = Sim800L.readSms(1);
  //Verifica se o SMS foi recebido e eh valido
  if (textSms.indexOf("OK") != -1)
  {
    //Verifica se o SMS esta vazio
    if (textSms.length() > 7)
    {
      //Retorna o numero do celular que enviou o SMS
      numberSms = Sim800L.getNumberSms(1);
      //Mostra o numero do telefone no Serial Monitor
      Serial.println(numberSms);
      //Converte o texto para maiusculas
      textSms.toUpperCase();
      //Caso receba L1, acende o led 1 (vermelho)
      if (textSms.indexOf("L1") != -1)
      {
        Serial.println("Led 1 Ligado");
        digitalWrite(LED1, 1);
      }
      //Caso receba D1, apaga o led 1 (vermelho)
      else if (textSms.indexOf("D1") != -1)
      {
        Serial.println("Led 1 Desligado");
        digitalWrite(LED1, 0);
      }
      //Caso receba L2, acende o led 2 (verde)
      else if (textSms.indexOf("L2") != -1)
      {
        Serial.println("Led 2 Ligado");
        digitalWrite(LED2, 1);
      }
      //Caso receba D2, apaga o led 2 (verde)
      else if (textSms.indexOf("D2") != -1)
      {
        Serial.println("Led 2 Desligado");
        digitalWrite(LED2, 0);
      }
      //Apaga todos as mensagens da memoria, garantindo que o proximo
      //SMS esteja sempre na posicao 1
      Sim800L.delAllSms();
    }
  }
}