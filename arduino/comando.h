#ifndef COMANDO_H_
#define COMANDO_H_

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

void lerComando(boolean *start, boolean *writeFile, File *fileWrite,File *fileRead){
  
    String recebeComandoInteiro = Serial.readString();
    
    String comando;
    
    int i = 0;
    boolean sair = false;
    
    while(!sair){
      
      if(recebeComandoInteiro[i] == '\0') sair = true;
     
      else if(recebeComandoInteiro[i] == ' ') sair = true;
      
      else{
        comando += recebeComandoInteiro[i];
        i++;
      }
    }
    
    if(comando == "write"){
      
      String nome;
      i++;
      while(recebeComandoInteiro[i] != '\0'){
        
        nome += recebeComandoInteiro[i];
        i++;
      }
      
      nome += ".txt";
      
      const char *nomeCompleto = nome.c_str();
      
      *fileWrite = SD.open(nomeCompleto, FILE_WRITE);
      
      if(!fileWrite){
       Serial.println("Erro ao criar o arquivo!"); 
      }
      else{
        
        *writeFile = true;
        Serial.println("escreveu o arquivo");
      }
    }
    
    else if(comando == "read"){
      
      String nome;
      i++;
      while(recebeComandoInteiro[i] != '\0'){
        
        nome += recebeComandoInteiro[i];
        i++;
      }
      
      nome += ".txt";
      
      const char *nomeCompleto = nome.c_str();
      
      *fileRead = SD.open(nomeCompleto);
      
      if(!(*fileRead)){
        
        Serial.println("Nao foi possivel ler o arquivo!");
      }else{
        
        Serial.println("read");
        
      }
    
    }
    
    else if(comando == "start"){
      
      *start = true;
      
      Serial.println("alterou o start");
    }
    
    else{
      
      Serial.println("ERRO! Comando nao reconhecido");
    }
    
}

#endif
