#ifndef SALVEFILE_H_
#define SALVEFILE_H_

#include "apagar.h"

void saving(EthernetClient *client){

  Serial.println("Creating File");
  
  limparCartao();
  
  File file = SD.open("mus.txt",FILE_WRITE);
  
  if(!file){
    
    Serial.println("Could not create file");
    return;
  }
  
  Serial.println("criou o arquivo");
  
  char c = client->read();
  
    while(c != '*'){ //ponto de parada
      
      Serial.write(c);
      file.print(c);
        
      c = client->read();
      
    }
  
  client->print("recebi");
  file.close();
  Serial.println("fechei o arquivo");
}
#endif
