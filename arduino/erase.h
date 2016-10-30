#ifndef ERASE_H
#define ERASE_H

//responsible function to delete the music saved on the card.
void cleaningCard(){
  
  Serial.println("erasing the music saved on the card!");
    
    if(SD.remove("mus.txt")){
      
      Serial.println("successful removal!");
    }else{
      
      Serial.println("Could not be removed, or the file does not exist!");
    }
}

#endif

