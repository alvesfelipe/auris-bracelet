#ifndef SALVEFILE_H_
#define SALVEFILE_H_

void saving(File *fileWrite, EthernetClient *client, File *theFile){

  Serial.println("Creating File");
  
  *theFile = SD.open("results.xml",FILE_WRITE);
  
  if(!(*theFile)){
    
    Serial.println("Could not create file");
    while(1);
  }
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client->available()) {
    char c = client->read();
    theFile->print(c);
  }else{
    
    Serial.println("nao available");
  }

  // if the server's disconnected, stop the client:
  if (!client->connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client->stop();
    theFile->close();
    Serial.println("Finished writing to file");
    
    // do nothing forevermore:
   // while (true);
  }
}
#endif
