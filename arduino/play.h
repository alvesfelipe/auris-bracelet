#ifndef PLAY_H_
#define PLAY_H_

#include <SD.h>
#include "Adafruit_TLC5947.h"

#include "erase.h"

#define NUM_TLC5947 1
#define DATA 7
#define CLOCK 6
#define LATCH 5

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5947, CLOCK, DATA, LATCH);


//variable that says, which is the port that the Arduino is connected.
EthernetServer server(3300);

//function responsible for playing the music on the bracelet
void startMusic(){
  
  int endTimeOfThePreviousEngine;//variable charge to save the final time the last note
  char value;//variable responsible to warn that the music switches over

  tlc.begin();
  tlc.write();
  
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
              cleaningCard();
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
        client_stop.stop(); //Closing the client connection
      }
      
      String line = ""; //Variable responsible for saving one line at a time from the file
      
      value = file.read();
      
      while(value != '\n'){
      
        line += value;
        value = file.read();
      }
      
      int space[4]; //Variable responsible for saving the position of each line space
      int e_commercial[23]; //Variable responsible for saving the position of each "&" line
      
      int e = 0; //Variable responsible for counting the amount of "&" contained in the line.
      
      for(int i = 0, p = 0; i < line.length(); i++){ //Responsible tie to save the positions of spaces and "&", in their corresponding variables.
      
        if(line[i] == ' '){
          
          space[p] = i;
          p++;
        }else if(line[i] == '&'){
        
          e_commercial[e] = i;
          e++;
        }
      }
      //The line variable must contain: motor(s), start time, final time, and intensity.
      
      //If the counter of "&" equals 0, that is, the musical note only needs 1 motor, make ...
      if(e == 0){
        
        endTimeOfThePreviousEngine = atol(line.substring(space[0],space[1]).c_str()) - endTimeOfThePreviousEngine;
                                     //startTime - endTimeOfThePreviousEngine;
                                     
        delay(endTimeOfThePreviousEngine);
        
        //assigning the intensity to the desired engine
        tlc.setPWM(atoi(line.substring(0,space[0]).c_str()),(atoi(line.substring(space[2]).c_str())*13.9f)+550);
        tlc.write();
        
        delay(atol(line.substring(space[1],space[2]).c_str()) - atol(line.substring(space[0],space[1]).c_str()));
                  //(finalTime - startTime)
                  
                  
        tlc.setPWM(atoi(line.substring(0,space[0]).c_str()),0);
                  //(motor,0);
        tlc.write();
        
        endTimeOfThePreviousEngine = atol(line.substring(space[1],space[2]).c_str());
                                    // = finalTime
      
    
      }else{
        
        endTimeOfThePreviousEngine = atol(line.substring(space[0],space[1]).c_str()) - endTimeOfThePreviousEngine;
                                     //startTime - endTimeOfThePreviousEngine;
                                     
        delay(endTimeOfThePreviousEngine);
        
        //assigning the intensity to the desired engine
        for(int i = 0; i <= e; i++){
          
          //  For the first engine
          if(i == 0) tlc.setPWM(atoi(line.substring(0,e_commercial[0]).c_str()),(atoi(line.substring(space[2]).c_str())*13.9f)+550);
          
          // For the engines before the last engine
          else if ( i < e) tlc.setPWM(atoi(line.substring(e_commercial[i-1]+1, e_commercial[i]).c_str()),(atoi(line.substring(space[2]).c_str())*13.9f)+550);
          
          //For the last engine
          else tlc.setPWM(atoi(line.substring(e_commercial[i-1]+1,space[0]).c_str()),(atoi(line.substring(space[2]).c_str())*13.9f)+550);
        }
        
        tlc.write();
        
        delay(atol(line.substring(space[1],space[2]).c_str()) - atol(line.substring(space[0],space[1]).c_str()));
                  //(finalTime - startTime)
        
        //assigning the intensity = 0 to the desired engine
        for(int i = 0; i <= e; i++){
          
          if(i == 0) tlc.setPWM(atoi(line.substring(0,e_commercial[0]).c_str()),0);
          
          else if ( i < e) tlc.setPWM(atoi(line.substring(e_commercial[i-1]+1, e_commercial[i]).c_str()),0);
          
          else tlc.setPWM(atoi(line.substring(e_commercial[i-1]+1,space[0]).c_str()),0);
        }
        
        tlc.write();
         // endTimeOfThePreviousEngine = finalTime
        endTimeOfThePreviousEngine = atol(line.substring(space[1],space[2]).c_str());
        
      }
      
    }
    //close the music
    file.close();
    //erase the music
    cleaningCard();  
}

#endif
