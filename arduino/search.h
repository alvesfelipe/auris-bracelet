#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "salveFile.h"

String nome = "";//Variable responsible for saving the melody name

void search(EthernetClient *client){
  
  nome = "";
  
  char partMusic = client->read();
  
  while(partMusic != '#'){
    //Except only name that are in the arduino ascii table
    if((partMusic >= '!')&&(partMusic <= '}')){
      nome += partMusic;
    }
    partMusic = client->read();
  }
  
  nome += ".txt";
  
  Serial.println(nome.c_str());

  //Check if this file already exists
  if(!SD.exists(&nome[0])){
    
    client->print("404");
    Serial.println("404 - File not found!");
    
    File file = SD.open(&nome[0],FILE_WRITE);
    
    if(!file){
      Serial.println("Did not create the file");
      client->print("412");
      return;
    }
    
    Serial.println("criou o arquivo");
    client->print("201");
    //If it does not, it will save the desired melody
    saving(client,file);

    file.close();
    Serial.println("File saved successfully");
        
    return;
  }
  
  client->print("yes");
  Serial.println("File found successfully!");
}


#endif
