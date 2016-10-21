#ifndef COMANDO_H_
#define COMANDO_H_

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
      
      if(i == 5){
        
        if(comando == "write"){
          
          client->println("comando aceito");
          saving(client);
        }else if(comando == "start"){
          
          client->stop();
          startMusic();
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
