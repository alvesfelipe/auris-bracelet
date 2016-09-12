#include <SD.h>

/*
 Pin board "Micro Card Adapter" to be connected to the ICSP pin's arduino:
 ** vcc - pin 2
 ** MOSI - pin 4
 ** MISO - pin 1
 ** CLrestart - pin 3
 ** CS - 
 ** GND - pin 6
*/ 


long endTimeOfThePreviousEngine = 0; //variable that stores the end time of the previous engine
long motor, intensity, startTime, finalTime; //variables that will save the parameters of each tone musical note
File myFile; //variable, whose purpose is to guard all the notes of the desired music.
//String recebe = "";
char value; //variable that will save the values of the parameters of the note temporarily
int sizeParameter = 0;
int restart = 0; //variable responsible for helping to restart the initial value, the parameter to be entered
int selection = 0; //responsible for variable tell which engine parameter content will be saved
char content[10] = {" "}; //variable responsible for saving the contents of all parameters, but one at a time


void setup(){
  
  //frequency on the serial port
  Serial.begin(9600);
  
  //wait until the serial port is available
  while(!Serial);
  
  //engine's pins to be used
  pinMode(9, OUTPUT);
  //pinMode(3, OUTPUT);
  //pinMode(4, OUTPUT);
  //pinMode(5, OUTPUT);
  //pinMode(6, OUTPUT);
  //pinMode(7, OUTPUT);
  
  //if not recognize the card reader
  if(!SD.begin(53)){
    Serial.println("initialization failed!");
    return;
  }  
  
  Serial.println("initialization done.");
  
  //save the desired entire song
  myFile = SD.open("music.txt");  
}

void loop(){
  
  //if saved a song, start reading
  if(myFile){
    
    while(myFile.available()){
      
      sizeParameter = 0;
      restart = 0;
      selection = 0;
      
      while(selection != 4){
        
        value = myFile.read();
        
        if((value == ' ')||(value == '\n')){
        
          if(selection == 0){
            
            motor = 9;//1 + atoi(content);
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
      
      Serial.println(millis());
      
      endTimeOfThePreviousEngine = finalTime;
      
      //motor = intensity = finalTime = startTime = 0;
      
    }
  
  }else{
    

    Serial.println("error opening the arquivo.");
  }
}

