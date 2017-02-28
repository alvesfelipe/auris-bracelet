#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "salveFile.h"

String nome = "";

void search(EthernetClient *client){
  
  nome = "";
  
  char partMusic = client->read();
  
  while(partMusic != '#'){
    
    if((partMusic >= '!')&&(partMusic <= '}')){
      nome += partMusic;
    }
    partMusic = client->read();
  }
  
  nome += ".txt";
  
  Serial.println(nome.c_str());
  
  if(!SD.exists(&nome[0])){
    
    client->print("not");
    Serial.println("File not found!");
    
    File file = SD.open(&nome[0],FILE_WRITE);
    
    if(!file){
      Serial.println("nao criou o arquivo");
      client->print("recebi");
      return;
    }
    
    Serial.println("criou o arquivo");
    
    saving(client,file);

    file.close();
    Serial.println("salvamento completo");
    //carregar(nome);
    
    return;
  }
  
  client->print("yes");
  //carregar(nome);
  Serial.println("File found successfully!");
  
}


#endif
