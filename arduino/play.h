#ifndef PLAY_H_
#define PLAY_H_

#include <SD.h>

//File fileRead;

void startMusic(EthernetClient *client){
  
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
          
      while(selection != 4){
        
        value = file.read();
        
        //stop
        if(*client){
          int j = 0;
          String comando;
          
          while(client->available()){
            
            comando += client->read();
            j++;
            
            if(j == 4){
              
              if(comando == "stop"){
                client->stop();
                return;
              }
            }
          }
        }
        
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
}

#endif
