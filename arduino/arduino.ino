#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include "Adafruit_TLC5947.h"

#include "command.h"

//address arduino's mac
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xD0, 0x93 };

void setup() {
  
  Serial.begin(9600);
  
  tlc.begin();
  tlc.write();
  
  while(!Serial);
  
  //Waiting until you get an ip
  if(Ethernet.begin(mac) == 0){
    
    Serial.println("failed to configure Ethernet using DHCP");
    
    while(true);
  }
  
  //start the server
  server.begin();
  
  //Show the serial port, the arduino's IP
  Serial.print("Server address: ");
  Serial.println(Ethernet.localIP());
  
  
  Serial.print(F("Initializing SD card..."));

  //initializing the card
  if (!SD.begin(4)) {
    Serial.println(F(" initialization failed!"));
    return;
  }
  Serial.println(F(" initialization done."));
  
  //cleaningCard();
}

//double tempo;

void loop() {
  
  //waiting until someone connect the arduino
  EthernetClient client = server.available();
  
  if(!client) return;
  
  //tempo = millis();
  
  Serial.println("client connects!");
  readCommand(&client);

  //tempo = millis() - tempo;
  //Serial.println(tempo);  
}

