// Gets a file from the Internet and saves it to the SD card
// Also saves the incoming HTTP header to the file
// This example gets the XML cricket score from synd.cricbuzz.com
//
// References - Arduino Example sketches from IDE by David A. Mellis et al.:
// -- File --> Examples --> Ethernet --> WebClient
// http://www.arduino.cc/en/Tutorial/WebClient
// -- File --> Examples --> SD --> ReadWrite
// http://www.arduino.cc/en/Tutorial/ReadWrite
// 
// Author: W.A. Smith    Date: 10 June 2015
// http://startingelectronics.org/software/arduino/save-web-file-to-SD-card/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#include "comando.h"
#include "salveFile.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "synd.cricbuzz.com";    // change server to get file from here
IPAddress ip(150, 165, 205, 160);
EthernetClient client;
File theFile;

void setup() {
  // disable Ethernet chip
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  
  Serial.begin(9600);
  
  Serial.print(F("Initializing SD card..."));

  if (!SD.begin(4)) {
    Serial.println(F(" initialization failed!"));
    return;
  }
  Serial.println(F(" initialization done."));
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(2000);
  
  Serial.println("connecting...");
  
  Serial.println(Ethernet.localIP());

  // if you get a connection, report back via serial:
  while(!(client.connect(server, 80)));
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /j2me/1.0/livematches.xml HTTP/1.1");  // change resource to get here
    client.println("Host: synd.cricbuzz.com");                 // change resource host here
    client.println("Connection: close");
    client.println();
  //}
  /*else {
    // didn't get a connection to the server:
    Serial.println("connection failed");
  }*/
  
  saving(NULL, &client, &theFile);
  
}

boolean start = false, writeFile = false, readFile = false;
File fileWrite, fileRead;

void loop() {
  
  //Pegar o comando
  if(Serial.available()){
    
   
    lerComando(&start,&writeFile,&fileWrite,&fileRead);
  }
  
  //salvar o arquivo
  if(writeFile){
    
    //saving(&fileWrite);
    Serial.println("Salvando!!");
    writeFile = false;
  }
  
  
  //tocar musica;
  if(start){
    
    
    Serial.println("Iniciando!!");
    
    start = false;
  }
  
  
}

