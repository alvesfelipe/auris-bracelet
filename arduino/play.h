#ifndef PLAY_H_
#define PLAY_H_

#include <SD.h>
#include "Adafruit_TLC5947.h"

#include "search.h"

#define NUM_TLC5947 1
#define DATA 7
#define CLOCK 6
#define LATCH 5

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5947, CLOCK, DATA, LATCH);

//variable that says, which is the port that the Arduino is connected.
EthernetServer server(3300);

//function responsible for playing the music on the bracelet
void startMusic(){
  
  double tempo = millis();
  
  Serial.println(nome.c_str());
  
  int endTimeOfThePreviousEngine;//variable charge to save the final time the last note
  char value;//variable responsible to warn that the music switches over
  
  const byte constant = 1;
  
  String line[constant] = ""; //Variable responsible for saving one line at a time from the file
  
  byte quant_line = 0;
  
  byte space[constant][4]; //Variable responsible for saving the position of each line space
  byte e_commercial[constant][23]; //Variable responsible for saving the position of each "&" line
      
  byte e[constant]; //Variable responsible for counting the amount of "&" contained in the line.
  
  //opening the saved music
  File file = SD.open(nome.c_str());
  
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
      
      //value = file.read();
      
      quant_line = 0;
      
      //reiniciando os valores
     while(quant_line < constant){
        line[quant_line] = "";
        for(int i = 0; i < 4; i++) space[quant_line][i] = 0;
        for(int i = 0; i < 23; i++) e_commercial[quant_line][i] = 0;
        e[quant_line] = 0;
        quant_line++;
      }
      
      quant_line = 0;
      
      //pegando 10 linhas do arquivo
      value = file.read();
      
      while(quant_line < constant){
        
        if(file.available()){
          
        //Serial.print("valor: ");
        //Serial.println(value);
          while(true){
            if(value != '\n'){
              line[quant_line] += value;
              value = file.read();
            }
            else{
              if(quant_line < (constant-1)) value = file.read();
              break;
            }
          }
        }
        quant_line++;
      }
      
      int var = quant_line;
      quant_line = 0;
      
      /*while(quant_line < var){ 
        Serial.println(line[quant_line].c_str());
        quant_line++;
      }*/
      
      
      while(quant_line < var){
      
        for(uint8_t i = 0, p = 0; i < line[quant_line].length(); i++){ //Responsible tie to save the positions of spaces and "&", in their corresponding variables.
      
          if(line[quant_line][i] == ' '){
          
            space[quant_line][p] = i;
            p++;
          }else if(line[quant_line][i] == '&'){
        
            e_commercial[quant_line][e[quant_line]] = i;
            e[quant_line]++;
          }
        }
        quant_line++;
      }
      
      quant_line = 0;
      
      while(quant_line < var){
        //Serial.println("***");
        
        //If the counter of "&" equals 0, that is, the musical note only needs 1 motor, make ...
        if(e[quant_line] == 0){
          
          //Serial.println(line[quant_line].c_str());
          
          //Serial.print("1: ");
          //Serial.println(atol(line[quant_line].substring(space[quant_line][0],space[quant_line][1]).c_str()) - endTimeOfThePreviousEngine);
          endTimeOfThePreviousEngine = atol(line[quant_line].substring(space[quant_line][0],space[quant_line][1]).c_str()) - endTimeOfThePreviousEngine;
                                       //startTime - endTimeOfThePreviousEngine;
          //Serial.println(endTimeOfThePreviousEngine);                            
          //if(endTimeOfThePreviousEngine > 200) endTimeOfThePreviousEngine = endTimeOfThePreviousEngine - 30;
          
          delay(endTimeOfThePreviousEngine);
          
          //assigning the intensity to the desired engine
          //Serial.print("2: ");
          //Serial.println(atoi(line[quant_line].substring(0,space[quant_line][0]).c_str()));
          tlc.setPWM(atoi(line[quant_line].substring(0,space[quant_line][0]).c_str()),(atoi(line[quant_line].substring(space[quant_line][2]).c_str())*13.9f)+550);
          tlc.write();
          
          //Serial.print("3: ");
          //Serial.println(atol(line[quant_line].substring(space[quant_line][1],space[quant_line][2]).c_str()) - atol(line[quant_line].substring(space[quant_line][0],space[quant_line][1]).c_str()));
          delay(atol(line[quant_line].substring(space[quant_line][1],space[quant_line][2]).c_str()) - atol(line[quant_line].substring(space[quant_line][0],space[quant_line][1]).c_str()));
                    //(finalTime - startTime)
                    
                    
          tlc.setPWM(atoi(line[quant_line].substring(0,space[quant_line][0]).c_str()),0);
                    //(motor,0);
          tlc.write();
          
          //Serial.print("4: ");
          //Serial.println(atol(line[quant_line].substring(space[quant_line][1],space[quant_line][2]).c_str()));
          endTimeOfThePreviousEngine = atol(line[quant_line].substring(space[quant_line][1],space[quant_line][2]).c_str());
                                      // = finalTime
        
      
        }else{
          
          Serial.println("n 0");
          
          endTimeOfThePreviousEngine = atol(line[quant_line].substring(space[quant_line][0],space[quant_line][1]).c_str()) - endTimeOfThePreviousEngine;
                                       //startTime - endTimeOfThePreviousEngine;
                                       
          delay(endTimeOfThePreviousEngine);
          
          //assigning the intensity to the desired engine
          for(int i = 0; i <= e[quant_line]; i++){
            
            //  For the first engine
            if(i == 0) tlc.setPWM(atoi(line[quant_line].substring(0,e_commercial[quant_line][0]).c_str()),(atoi(line[quant_line].substring(space[quant_line][2]).c_str())*13.9f)+550);
            
            // For the engines before the last engine
            else if ( i < e[quant_line]) tlc.setPWM(atoi(line[quant_line].substring(e_commercial[quant_line][i-1]+1, e_commercial[quant_line][i]).c_str()),(atoi(line[quant_line].substring(space[quant_line][2]).c_str())*13.9f)+550);
            
            //For the last engine
            else tlc.setPWM(atoi(line[quant_line].substring(e_commercial[quant_line][i-1]+1,space[quant_line][0]).c_str()),(atoi(line[quant_line].substring(space[quant_line][2]).c_str())*13.9f)+550);
          }
          
          tlc.write();
          
          delay(atol(line[quant_line].substring(space[quant_line][1],space[quant_line][2]).c_str()) - atol(line[quant_line].substring(space[quant_line][0],space[quant_line][1]).c_str()));
                    //(finalTime - startTime)
          
          //assigning the intensity = 0 to the desired engine
          for(int i = 0; i <= e[quant_line]; i++){
            
            if(i == 0) tlc.setPWM(atoi(line[quant_line].substring(0,e_commercial[quant_line][0]).c_str()),0);
            
            else if ( i < e[quant_line]) tlc.setPWM(atoi(line[quant_line].substring(e_commercial[quant_line][i-1]+1, e_commercial[quant_line][i]).c_str()),0);
            
            else tlc.setPWM(atoi(line[quant_line].substring(e_commercial[quant_line][i-1]+1,space[quant_line][0]).c_str()),0);
          }
          
          tlc.write();
           // endTimeOfThePreviousEngine = finalTime
          endTimeOfThePreviousEngine = atol(line[quant_line].substring(space[quant_line][1],space[quant_line][2]).c_str());
          
        }
        
        quant_line++;
      }
      
    }
    
    tempo = millis() - tempo;
    Serial.println(tempo);
    //close the music
    file.close();
    //erase the music
    //cleaningCard();  
}

#endif
