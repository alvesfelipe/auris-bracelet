#ifndef COMANDO_H_
#define COMANDO_H_

#include "play.h"
#include "salveFile.h"
#include "search.h"

//function that will read the command sent by the client
void readCommand(EthernetClient *client){
  
  //complete command
  String command;
  byte commandSize = 0;
  
  while(client->connected()){
    
    //variable will receive byte for byte the command passed by the client
    char partCommand;
    
    while(client->available()){
      
      commandSize++;
      partCommand = client->read();
      command += partCommand;
      
      if(commandSize == 5){
        
        if(command == "start"){ //To start the chosen melody
          
          //disconnect the client Arduino
          client->stop();
          startMusic();
          
        }else if(command == "searc"){//Command that will search the melody, and leave ready for the start
          
          Serial.println("Search");
          search(client);        
        }
        
        else{
          
          Serial.println("Unrecognized command!");
        }
        
        Serial.println("disconnecting the client!");
        client->stop();
      }
    }
  }
}

#endif
