#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "salveFile.h"
#include "carregar.h"

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
  
  char nome2[nome.length()-1];
  
  byte i = 0;
  
  while(i < nome.length()){
    nome2[i] = nome[i];
    i++;
  }
  nome2[i] = '\0';
  
  
  if(!SD.exists(nome2)){
    
    client->print("not");
    Serial.println("File not found!");
    
    File file = SD.open(nome2,FILE_WRITE);
    
    if(!file){
      Serial.println("nao criou o arquivo");
      client->print("recebi");
    }
    Serial.println("criou o arquivo");
    
    saving(client,file);
    Serial.println("salvamento completo");
    carregar(nome);
    
    return;
  }
  
  client->print("yes");
  carregar(nome);
  Serial.println("File found successfully!");
  
}


#endif
