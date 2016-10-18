#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#include "comando.h"
#include "salveFile.h"
#include "play.h"

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xD0, 0x93 };
EthernetServer server(3300);

void setup() {
  
  Serial.begin(9600);
  
  while(!Serial);
  
  if(Ethernet.begin(mac) == 0){
    
    Serial.println("failed to configure Ethernet using DHCP");
    
    while(true);
  }
  
  server.begin();
  
  Serial.print("Server address: ");
  Serial.println(Ethernet.localIP());
  
  
  Serial.print(F("Initializing SD card..."));

  if (!SD.begin(4)) {
    Serial.println(F(" initialization failed!"));
    return;
  }
  Serial.println(F(" initialization done."));
  
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  
  for(int i = 2; i <=7; i++) analogWrite(i,0);
  
  limparCartao();
}

void loop() {
  
  EthernetClient client = server.available();
  
  if(!client) return;
  
  Serial.println("Cliente conectou!");
  
  //Pegar comando
  lerComando(&client);
  
}

void limparCartao(){
  
  Serial.println("Apagando o arquivo");
    
    if(SD.remove("mus.txt")){
      
      Serial.println("Remocao bem sucedida!");
    }else{
      
      Serial.println("Nao foi possivel remover!");
    }
}

