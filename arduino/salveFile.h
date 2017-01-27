#ifndef SALVEFILE_H_
#define SALVEFILE_H_

#include "erase.h"

//function responsible for saving music on the card.
void saving(EthernetClient *client){
  //erase the music
  cleaningCard(); 
  
  
  Serial.println("create the file!");
  
  char partMusic = client->read();
  
  String nomeMusica = "";
  
  while(partMusic != '#'){
    
    nomeMusica += partMusic;
    partMusic = client->read();
  }
  
  nomeMusica += ".txt";
  
  //create the file that will store music
  File file = SD.open(nomeMusica.c_str(),FILE_WRITE);
  
  if(!file){
    
    Serial.println("Could not create file");
    return;
  }
  
  Serial.println("Nome da Musica salva com sucesso!");
  Serial.println("Pegando a melodia");
  
  //until it find the stopping point, or the client is connected
  while(partMusic != '#'){
    
    //will only save the music numbers, spaces, line breaks and points.
    if(((partMusic>='0')&&(partMusic<='9'))||
        (partMusic == ' ')||(partMusic == '\n')||
        (partMusic == '.')||(partMusic == '&')){
      
      //saved in the file
      file.print(partMusic);
      Serial.print(partMusic);
    }
    partMusic = client->read();
      
  }
 
  client->print("recebi");
  client->stop();
  file.close();
  
}
#endif
