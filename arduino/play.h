#ifndef PLAY_H_
#define PLAY_H_

#include <SD.h>

#include "apagar.h"

//File fileRead;
EthernetServer server(3300);

void startMusic(){
  
  int endTimeOfThePreviousEngine;
  int sizeParameter;
  int restart;
  int selection;
  char value;
  long motor, intensity, startTime, finalTime;
  char content[10] = {" "}; 
  
  File file = SD.open("mus.txt");
  
  if(!file){
    
    Serial.println("Nao encontrei o arquivo");
    return;
  }
  
  endTimeOfThePreviousEngine = 0;
    
    while(file.available()){

      sizeParameter = 0;
      restart = 0;
      selection = 0;
      
      EthernetClient client_stop = server.available();
      
      if(client_stop){
        
        int i = 0;
        String comando;
        char c;
        while(client_stop.available()){
          
          i++;
          c = client_stop.read();
          comando += c;
          
          if(i == 4){
            
            if(comando == "stop"){
              
              while(file.available()) file.read();
              file.close();
              limparCartao();
              client_stop.stop();
              return;
            }
            else{
              Serial.println("Comando nao reconhecido");
            }
          }
        
        }
        client_stop.stop();
      }
          
      while(selection != 4){
        
        value = file.read();
        
        if((value == ' ')||(value == '\n')){
        
          if(selection == 0){
            
            motor = 2 + atoi(content);
          }
          else if(selection == 1){
            
            intensity = atoi(content);
          }
          else if(selection == 2){
            
            startTime = atoi(content);
          }
          else if(selection == 3){
            
            finalTime = atoi(content);
          }
          selection++;
          sizeParameter = 0;
          
          for(restart=0; restart < 10; restart++) content[restart] = ' ';
          
        }else{
        
          content[sizeParameter] = value;
          sizeParameter++;
        }
      }
      
      endTimeOfThePreviousEngine = startTime - endTimeOfThePreviousEngine;
      
      delay(endTimeOfThePreviousEngine);
      
      analogWrite(motor, intensity);
      
      delay(finalTime-startTime);
      
      analogWrite(motor, 0);
        
      endTimeOfThePreviousEngine = finalTime;
      
    }
    file.close();
    limparCartao();  
}

#endif
