#include <ESP8266WiFi.h>
#include <EEPROM.h>
char inData[128];          //create 128 char array
byte indexcount = 0;           //index byte (0 - 255)
String string_convert;    //String 
String PCdata[20];           //String array
char wifissid [32];
char wifipassword [32];
char ntpserver [100];
byte gtmtime;
byte syncinterval;
char buffer[100];
char serveradres [100];
  
char ssid[32] = "";    //  your network SSID (name) 
char password[32] = "";
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  EEPROM.begin(512);
  Serial.begin(9600);
  delay(10);
  readEEPROM();
  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to "); 
  for(int x = 0; x <32; x++){
     Serial.print(ssid[x]);
 //    ssid[x] = wifissid[x];
 //    password[x] = wifipassword[x];
  }
  Serial.println();
  WiFi.begin(ssid, password);
  
/* while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }*/
  int wi_fi_timeout = 0;
  bool stat = true;
  while (stat){
    delay(500);
    wi_fi_timeout++;
    Serial.print("."); 
    if(WiFi.status() == WL_CONNECTED || wi_fi_timeout == 10){
      stat = false; 
    } 
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("");
    Serial.println("WiFi connected"); 
  }else if(wi_fi_timeout == 10 || WiFi.status() != WL_CONNECTED){
    Serial.println("");
    Serial.println("WiFi NOT connected, conection timeout!"); 
  }
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  Serial.println("###RTRDTA###");
}

void loop() {
  // Check if a client has connected
  parsing();
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
void parsing() {
  while (Serial.available() > 0) { //while there is data in serial buffer (128 bytes)
    char aChar = Serial.read();    //aChar - current symvol in buffer
    if (aChar != '#') {            //if aChar isnt the '#' (End) symbol do:
      inData[indexcount] = aChar;       //(inData - array, indexcount - loop № counter) write all Buffer to the array
      indexcount++;                     //indexcount + 1 - loop count
      inData[indexcount] = '\0';        //set the next position to '\0' or null symbol
    } else {                       //if aChar or current buffer symbol is 'E' (End) do:
      char *p = inData;            //create inData adress pointer
      char *str;                   //create pointer
      indexcount = 0;                   //set counter to 0
      String value = "";           //careate string
      while ((str = strtok_r(p, ";", &p)) != NULL) { 
        string_convert = str;                       
        PCdata[indexcount] = string_convert;      
        indexcount++;                                     //counter + 1 - loop count
      }
      indexcount = 0;                                     //counter to 0
      digitalWrite(13, HIGH);
      for(int x = 0; x < 20; x++){
        //Serial.println(PCdata[x]);
        int leng = 0;
        switch (x){
          case 0:
          leng = PCdata[x].length() + 1;
          PCdata[x].toCharArray(wifissid, leng);
          for(int i = 0; i < 32; i++){
             EEPROM.write(i,wifissid[i]);//0-32 ssid
             Serial.print(wifissid[i]);            
          }
          Serial.println();   
          break;
          case 1:
          leng = PCdata[x].length() + 1;
          PCdata[x].toCharArray(wifipassword, leng);
          for(int i = 0; i < 32; i++){
            EEPROM.write(i+32,wifipassword[i]);//32-64 password
            Serial.print(wifipassword[i]);
          }
          Serial.println();  
          break;
          case 2:
          gtmtime = PCdata[x].toInt();
          EEPROM.write(65,gtmtime);
          Serial.println(gtmtime);//65 gmt           
          break;
          case 3:
          syncinterval = PCdata[x].toInt();
          EEPROM.write(66,syncinterval);
          Serial.println(syncinterval);//66 sync
          break;
          case 4:
          PCdata[x].toCharArray(ntpserver, 100);
          for(int i = 0; i < 100; i++){
            Serial.print(ntpserver[i]);//67-167 ntpserver
            EEPROM.write(i+67,ntpserver[i]);
          }
          Serial.println();
          break;
        }      
      }
    }
  }
  EEPROM.commit();
}
void readEEPROM(){
  for(int x = 0; x < 32; x++){
    buffer[x] = EEPROM.read(x);
    Serial.print(buffer[x]);
    ssid[x] = buffer[x];
  }
  Serial.println();
  for(int x = 0; x < 32; x++){
    buffer[x] = EEPROM.read(x + 32);
    Serial.print(buffer[x]);
   password[x] = buffer[x];
  }
  Serial.println();
  gtmtime = EEPROM.read(65);
  Serial.println(gtmtime);
  syncinterval = EEPROM.read(66);
  Serial.println(syncinterval);
  for(int i = 0; i < 100; i++){
    buffer[i] = EEPROM.read(i+67);
    Serial.print(buffer[i]);//67-167 ntpserver
    serveradres[i] = buffer[i];
  }
  Serial.println();
}

