#include <ESP8266WiFi.h>
#include <bits/stdc++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Ticker.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
 
class Consulta {
private:
    String ID_usuario;
    String Data;
    String mensagem;

public:
    // Construtor
    Consulta(const String& id, const String& data, const String& msg) 
        : ID_usuario(id), Data(data), mensagem(msg) {}

    // Métodos para acessar os atributos
    String getIDUsuario() const {
        return ID_usuario;
    }

    String getData() const {
        return Data;
    }

    String getMensagem() const {
        return mensagem;
    }

    // Métodos para definir os atributos
    void setIDUsuario(const String& id) {
        ID_usuario = id;
    }

    void setData(const String& data) {
        Data = data;
    }

    void setMensagem(const String& msg) {
        mensagem = msg;
    }
};

String ID_USER = "1";
std::vector<Consulta> consultas;

const char* ssid = "Anisio-Vivo";
const char* password = "fernandesmontes";
WiFiClient client;
int LED_PIN = D4;

//PHONE = "+ZZxxxxxxxxxx";
String PHONE = "";
String msg = "";
String timeStamp = "";

//GSM Module RX pin to Arduino 3
//GSM Module TX pin to Arduino 2
int rxPin = D2;
int txPin = D3;
SoftwareSerial sim800(txPin, rxPin);

WiFiServer server(80);

void setup() {

  pinMode(LED_PIN, OUTPUT); //Setting Pin 13 as output
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Initializing Serial... ");
  delay(10);
 
  // Conecta a sua Rede WIFI e Mostra na Porta Serial
  Serial.println();
  Serial.println();
  Serial.print("Conectando a rede ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  Serial.println("");
 
}

void loop() {
  int cont = 0;
  while (WiFi.status() != WL_CONNECTED && cont <= 60) {
    delay(100);
    cont++;
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi conectado");
    if(!consultas.empty()){
      for(Consulta c : consultas){
        makePost(c,"adequate-engaged-calf.ngrok-free.app:8080/consultas");
      }
    }
  }

  while (sim800.available())
  {
    parseData(sim800.readString());//Calls the parseData function to parse SMS
  }
  doAction();//Does necessary action according to SMS message
  while (Serial.available())
  {
    sim800.println(Serial.readString());
  }
}

void parseData(String buff) {
  Serial.println(buff);

  unsigned int index;

  //Remove sent "AT Command" from the response string.
  index = buff.indexOf("\r");
  buff.remove(0, index + 2);
  buff.trim();

  if (buff != "OK") {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();

    buff.remove(0, index + 2);

    //Parse necessary message from SIM800L Serial buffer string
    if (cmd == "+CMT") {
      //get newly arrived memory location and store it in temp
      index = buff.lastIndexOf(0x0D);//Looking for position of CR(Carriage Return)
      msg = buff.substring(index + 2, buff.length());//Writes message to variable "msg"
      msg.toLowerCase();//Whole message gets changed to lower case
      Serial.println(msg);

      index = buff.indexOf(0x22);//Looking for position of first double quotes-> "
      PHONE = buff.substring(index + 1, index + 14); //Writes phone number to variable "PHONE"
      timeStamp = buff.substring(index + 21, index + 41);

      Serial.println(PHONE);
      Serial.println(timeStamp);

    }
  }
}

void doAction()
{

  if (msg == "led on")
  {
    digitalWrite(LED_PIN, HIGH);
    Consulta c(ID_USER, timeStamp, msg);
    consultas.emplace_back(c);
    Reply("LED is ON");
  }
  else if (msg == "led off")
  {
    digitalWrite(LED_PIN, LOW);
    Consulta c(ID_USER, timeStamp, msg);
    consultas.emplace_back(c);
    Reply("LED is OFF");
  }

  PHONE = "";//Clears phone string
  msg = "";//Clears message string
  timeStamp = "";
}

void Reply(String text)
{
  sim800.print("AT+CMGF=1\r");
  delay(1000);
  sim800.print("AT+CMGS=\"" + PHONE + "\"\r");
  delay(1000);
  sim800.print(text);
  delay(100);
  sim800.write(0x1A); //ascii code for ctrl+z, DEC->26, HEX->0x1A
  delay(1000);
  Serial.println("SMS Sent Successfully.");
}

void makePost(Consulta c, String url){
  yield();
  if (WiFi.status() != WL_CONNECTED) {
    HTTPClient http;
    if (http.begin(client, url)) {
      Serial.println("http.begin ok");
    }
    delay(100);
    http.addHeader("Content-Type", "application/json");
    String JeySon = "{";
    JeySon += "\"idUser\": " + c.getIDUsuario() + ",";
    JeySon += "\"mensagem\": " + c.getMensagem() + ",";
    JeySon += "\"data\": " + c.getData();
    JeySon += "}";
    Serial.println(JeySon);
    int httpCode = http.POST(JeySon);
    
    Serial.println(httpCode);
    if (httpCode > 0) { 
        const String& payload = http.getString();
        Serial.println(httpCode); 
        Serial.println(payload);
      switch (httpCode) {
        yield();
        case 200:
            Serial.println("Requisicao concluída com sucesso!");
          break;

        case 201:
        Serial.println("Requisicao id2 POST!");
        break;

        case 404:
        Serial.println("Site nao encontrado!");
        break;

        default:
          break;
      }
    }
    else {
      Serial.println("FAIL REQUEST");
    }
    http.end();
  } else {
    Serial.println("FALHA NA CONEXAO");
  }
}