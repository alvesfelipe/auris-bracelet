#ifndef PLAY_H_
#define PLAY_H_

#include <SD.h>
#include "Adafruit_TLC5947.h"

#include "search.h"
#include "carregar.h"

#define NUM_TLC5947 1
#define DATA 7
#define CLOCK 6
#define LATCH 5

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5947, CLOCK, DATA, LATCH);

//variable that says, which is the port that the Arduino is connected.
EthernetServer server(3300);

//function responsible for playing the music on the bracelet
void startMusic(){
  
  unsigned int tempo = millis();
  
  unsigned int tempo_real = millis();
    //while there note to be played, do ...
  Comando *comando_atual = listaDeComando.comando;
    while(comando_atual != NULL){
           
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
              
              //while(file.available()) file.read();
              //close the music
              //file.close();
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
      //comando_atual
      while((tempo_real + comando_atual->inicio ) > millis());

      Serial.print(tempo_real + comando_atual->inicio);
      Serial.print(" ");
      Serial.println(millis());
      
      for(int i = 0; i < 24; i++){
        
        if(comando_atual->motores[i] == true) tlc.setPWM(i,comando_atual->intensidade);
      }
      tlc.write();
      
      while((tempo_real + comando_atual->fim) > millis());
      
      for(int i = 0; i < 24; i++){
        
        if(comando_atual->motores[i] == true) tlc.setPWM(i,0);
      }
      tlc.write();
      
      comando_atual = comando_atual->prox;
      
    }
    
    tempo = millis() - tempo;
    Serial.print("tempo: ");
    Serial.println(tempo);
    removerAll(&listaDeComando);
    Serial.println("Apagou a lista de comandos salvos");
}

#endif
