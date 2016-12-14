#ifndef SALVEFILE_H_
#define SALVEFILE_H_

#include "erase.h"

//function responsible for saving music on the card.
void saving(EthernetClient *client){

  Serial.println("Creating File");
  
  //erase the music
  cleaningCard(); 
  
  //create the file that will store music
  File file = SD.open("mus.txt",FILE_WRITE);
  
  if(!file){
    
    Serial.println("Could not create file");
    return;
  }
  
  Serial.println("create the file!");
  
  char partMusic = client->read();
  
  //until it find the stopping point, or the client is connected
  while((partMusic != '*')&&(client->available())){
    
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
