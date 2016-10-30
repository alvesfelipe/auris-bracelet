#ifndef COMANDO_H_
#define COMANDO_H_

#include "play.h"
#include "salveFile.h"

//function that will read the command sent by the client
void readCommand(EthernetClient *client){
  
  //complete command
  String command;
  int commandSize = 0;
  
  while(client->connected()){
    
    //variable will receive byte for byte the command passed by the client
    char partCommand;
    
    while(client->available()){
      
      commandSize++;
      partCommand = client->read();
      command += partCommand;
      
      if(commandSize == 5){
        
        if(command == "write"){
          
          //notice to you that I accepted the command
          client->println("command accepted");
          saving(client);
          
        }else if(command == "start"){
          
          //disconnect the client Arduino
          client->stop();
          startMusic();
        }else{
          
          Serial.println("Unrecognized command!");
        }
        
        Serial.println("disconnecting the client!");
        client->stop();
      }
    }
  }
}

#endif
