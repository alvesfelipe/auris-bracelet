#ifndef SALVEFILE_H_
#define SALVEFILE_H_

void saving(EthernetClient *client){

  //File theFile;
  
  Serial.println("Creating File");
  
  File file = SD.open("mus.txt",FILE_WRITE);
  
  if(!file){
    
    Serial.println("Could not create file");
    return;
  }
  
  Serial.println("criou o arquivo");
  
  char c = client->read();
  //c = client->read();
  //while(client->connected()){
    while(c != '*'){
      
      //if(c != 152){
        Serial.write(c);
        file.print(c);
        
        c = client->read();
      //}else{
        //c = client->read();
      //}
    }
    //client->read();
  //}
  
  client->print("recebi");
  file.close();
  Serial.println("fechei o arquivo");
}
#endif
