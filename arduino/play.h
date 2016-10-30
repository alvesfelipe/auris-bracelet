#ifndef PLAY_H_
#define PLAY_H_

#include <SD.h>

#include "erase.h"

//variable that says, which is the port that the Arduino is connected.
EthernetServer server(3300);

//function responsible for playing the music on the bracelet
void startMusic(){
  
  int endTimeOfThePreviousEngine;//variable charge to save the final time the last note
  int sizeParameter;//variable responsible to assist cleaning the content
  int restart;//variable responsible to assist cleaning the content
  int selection;//variable responsible to select where in which the parameters the content will be saved.
  char value;//variable responsible to warn that the music switches over
  long motor, intensity, startTime, finalTime; //parameters of the note to be played
  char content[10] = {" "}; //variable responsible for keeping one of music's parameters
  
  //opening the saved music
  File file = SD.open("mus.txt");
  
  if(!file){
    
    Serial.println("Nao encontrei o arquivo");
    return;
  }
  
  //starting the start time to zero
  endTimeOfThePreviousEngine = 0;
  
    //while there note to be played, do ...
    while(file.available()){
     
      sizeParameter = 0;
      restart = 0;
      selection = 0;
      
      //if the client connect, while the music is playing
      EthernetClient client_stop = server.available();
      
      if(client_stop){
        
        int commandSize = 0;
        String command;
        char partCommand;
        while(client_stop.available()){
          
          commandSize++;
          partCommand = client_stop.read();
          command += partCommand;
          
          if(commandSize == 4){
            
            //verify that the received command is the command to stop the music
            if(command == "stop"){
              
              while(file.available()) file.read();
              //close the music
              file.close();
              //erase the music
              cleaningCard()
              //disconnect the customer
              client_stop.stop();
              //and finalize the function of playing music
              return;
            }
            else{
              Serial.println("Unrecognized command!");
            }
          }
        
        }
        client_stop.stop();
      }
          
      while(selection != 4){
        
        value = file.read();
        
        if((value == ' ')||(value == '\n')){
        
          if(selection == 0){
            
            switch(atol(content)){
              case 0:
                motor = 2;
                break;
              case 1:
                motor = 3;
                break;
              case 2:
                motor = 5;
                break;
              case 3:
                motor = 6;
                break;
              case 4:
                motor = 7;
                break;
              case 5:
                motor = 8;
                break;
              default:
                motor = 2;
                break;
            }
            
          }
          else if(selection == 3){
            
            intensity = atol(content);
          }
          else if(selection == 1){
            
            startTime = atol(content);
          }
          else if(selection == 2){
            
            finalTime = atol(content);
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
      
      analogWrite(motor, intensity);//start the note
      
      delay(finalTime-startTime);
      
      analogWrite(motor, 0);//Finish the note
        
      endTimeOfThePreviousEngine = finalTime;
      
    }
    //close the music
    file.close();
    //erase the music
    cleaningCard();  
}

#endif
