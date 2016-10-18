#ifndef COMANDO_H_
#define COMANDO_H_

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#include "play.h"
#include "salveFile.h"

void lerComando(EthernetClient *client){
  
  String comando;
  int i = 0;
  
  while(client->connected()){
    
    char c;
    
    while(client->available()){
      
      i++;
      c = client->read();
      comando += c;
      Serial.println(c);
      
      if(i == 5){
        
        if(comando == "write"){
          
          client->println("comando aceito");
          saving(client);
        }else if(comando == "start"){
          
          client->stop();
          startMusic(client);
          
          Serial.println("Apagando o arquivo");
    
          if(SD.remove("mus.txt")){
      
            Serial.println("Remocao bem sucedida!");
          }else{
      
            Serial.println("Nao foi possivel remover!");
          }
        }else{
          
          Serial.println("Comando nao reconhecido");
        }
        
        Serial.println("desconectando o cliente!");
        client->stop();
      }
    }
  }
}

#endif
