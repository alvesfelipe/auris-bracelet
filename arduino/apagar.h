#ifndef APAGAR_H
#define APAGAR_H

void limparCartao(){
  
  Serial.println("Apagando o arquivo");
    
    if(SD.remove("mus.txt")){
      
      Serial.println("Remocao bem sucedida!");
    }else{
      
      Serial.println("Nao foi possivel remover!");
    }
}

#endif

