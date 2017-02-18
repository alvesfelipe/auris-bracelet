#ifndef CARREGAR_H
#define CARREGAR_H

#include "listaComando.h"

Lista listaDeComando;
 
void carregar(String name_){

  boolean motores[24];
  unsigned int inicio, fim;
  int intensidade;
  
  removerAll(&listaDeComando);
  Serial.println("Apagou a lista de comandos salvos");
  criar(&listaDeComando);
  
  File file = SD.open(name_.c_str());
  
  if(!file){
    
    Serial.println("Nao conseguiu abrir a melodia.");
    return;
  }
  
  Serial.print("Carregando a musica: ");
  Serial.println(name_.c_str());
  
  char value;
  String line;
  while(file.available()){

    for(int i =0; i < 24; i++) motores[i] = false;
    inicio = fim = 0;
    intensidade = 0;
    
    line = "";  
    
    value = file.read();
    
    while(value != '\n'){
      line += value;
      value = file.read();
    }
    
    byte space[5];
    byte e_commercial[23];
    byte e = 0;
    
    Serial.println(line.c_str());
    
    for(uint8_t i = 0, p = 0; i < line.length(); i++){ //Responsible tie to save the positions of spaces and "&", in their corresponding variables.
      
      if(line[i] == ' '){
          
        space[p] = i;
        p++;
      }else if(line[i] == '&'){
        
        e_commercial[e] = i;
        e++;
      }
    }
    
    if(e>0){
      for(int i = 0; i < e; i++){
      
        if(i == 0) motores[atoi(line.substring(0,e_commercial[0]).c_str())] = true;
      
        else if(i<e) motores[atoi(line.substring(e_commercial[i-1]+1, e_commercial[i]).c_str())] = true;
    
        else motores[atoi(line.substring(e_commercial[i-1]+1,space[0]).c_str())] = true;
      }
    }
    else{
      motores[atoi(line.substring(0,space[0]).c_str())] = true;
      
    }
    
    inicio = atol(line.substring(space[0],space[1]).c_str());
    
    fim = atol(line.substring(space[1],space[2]).c_str());
    
    intensidade = (atoi(line.substring(space[2]).c_str())*13.9f)+550;
     line.remove(0);

    if(!inserir(&listaDeComando,motores,inicio,fim,intensidade)){

      Serial.println("Echeu a memoria");
      break;
    }
    
  }
  
  file.close();
}

#endif
