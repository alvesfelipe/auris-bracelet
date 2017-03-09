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
  
  double tempo_real = millis();

  File file = SD.open(nome.c_str());//open the chosen melody
  
  if(!file){
    Serial.println("Did not upload the song");
    return;
  }
  
  char value; //Variable that will save a byte of the melody
  
  while(file.available()){

    //Variable responsible for saving one line of the melody at a time
    String line = "";

    value = file.read();//Read one byte
    
    while(value != '\n'){
      //Saved only in "line" the desired values
      if(((value>='0')&&(value<='9'))||
        (value == ' ')||(value == '.')||(value == '&')){
        
        line += value;
      }
      value = file.read();
    }

    //Variable responsible for saving the position of each space contained in the line
    int space[4];
    //Variable responsible for saving the position of each '&' contained in line
    int e_commercial[23];
    //Variable responsible for saving the amount of & contained in line
    int e = 0;

    //Responsible for indicating the values of "space", "e_commercial" and "e" in the line
    for(int i = 0, p = 0; i < line.length(); i++){

      if(line[i] == ' '){

        space[p] = i;
        p++;
      }else if(line[i] == '&'){

        e_commercial[e] = i;
        e++;
      }
    }
    
    if(e == 0){

      double tempo_inicio = atol(line.substring(space[0],space[1]).c_str());
       
      while(tempo_real + tempo_inicio > millis());

      tlc.setPWM(atoi(line.substring(0,space[0]).c_str()),(atoi(line.substring(space[2]).c_str())*13.9f)+550);
      tlc.write();

      double tempo_fim = atol(line.substring(space[1],space[2]).c_str());

      while(tempo_real + tempo_fim > millis());

      tlc.setPWM(atoi(line.substring(0,space[0]).c_str()),0);
      tlc.write();

    }else{

      double tempo_inicio = atol(line.substring(space[0],space[1]).c_str());

      while(tempo_real + tempo_inicio > millis());

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

      double tempo_fim = atol(line.substring(space[1],space[2]).c_str());
      
      while(tempo_real + tempo_fim > millis());

      
      //assigning the intensity to the desired engine
      for(int i = 0; i <= e; i++){

        //  For the first engine
        if(i == 0) tlc.setPWM(atoi(line.substring(0,e_commercial[0]).c_str()),0);

        // For the engines before the last engine
        else if ( i < e) tlc.setPWM(atoi(line.substring(e_commercial[i-1]+1, e_commercial[i]).c_str()),0);

        //For the last engine
        else tlc.setPWM(atoi(line.substring(e_commercial[i-1]+1,space[0]).c_str()),0);
        
      }
      tlc.write();

    }
    //delete the line content
    line.remove(0);
  }
  file.close();
  tempo_real = millis() - tempo_real;
  Serial.print("tempo: ");
  Serial.println(tempo_real);
  SD.remove("musicaV.txt");
  Serial.println("Apagou a lista de comandos salvos");
}

#endif
