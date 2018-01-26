
/*-------- libraries ----------*/

#include <ESP8266WiFi.h>
#include <LedControlMS.h>
#include <DS1307.h>
#include "ThingSpeak.h"
#include <DHT.h>
#include <Wire.h>
#include <BMP180.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

/*-------- EEPROM variables ----------*/

char inData[128];          
byte indexcount = 0;          
String string_convert;    
String PCdata[20];           
char wifissid [32];
char wifipassword [32];
char ntpserver [100];
byte gtmtime;
byte syncinterval;
char buffer[100];
char serveradres [100];
char ssid[32] = "";     
char password[32] = "";

/*-------- Definition ----------*/

extern "C" {
    #include "user_interface.h"
}
#define DHTPIN 13     
#define DHTTYPE DHT22 
BMP180 barometer;
DHT dht(DHTPIN, DHTTYPE);
DS1307  rtc(12,  14);
Time  t;

/*-------- Variables ----------*/

unsigned long last_millis;
unsigned long s_last_millis;
float h;
float temp;
float p;
float pt;
boolean negative = false;
boolean dot;
boolean blinker = true;
int dayofweek;
int minutes = t.min;
int hours = t.hour;
int seconds = t.sec;
int value;
unsigned long inpu = 0;
int output = 0;
int number = 0;
int timeZone = 2;

/*-------- NTP setup ----------*/

WiFiUDP Udp;
unsigned int localPort = 8888;
time_t getNtpTime();
void digitalClockDisplay();
void sendNTPpacket(IPAddress &address);
time_t prevDisplay = 0;

/*-------- Set up 7-seg driver ----------*/

LedControl lc = LedControl(0,2,15,3);

/*-------- Set up thingspaek ----------*/

unsigned long myChannelNumber = YourNumber;
const char * myWriteAPIKey = "YourAPIKey";
int status = WL_IDLE_STATUS;
WiFiClient  client;

/*-------- Setup ----------*/

void setup() { 
  
  /*-------- Start all modules ----------*/
  
  Serial.begin(9600);
  EEPROM.begin(512);
  dht.begin(); 
  rtc.halt(false);
  Wire.begin();

  /*-------- Read the EEPROM data ----------*/
  
  delay(10);
  readEEPROM();

  /*-------- Test the barometer ----------*/
  
  barometer = BMP180();
  if(barometer.EnsureConnected())
  {
    Serial.println("Connected to BMP180.");
    barometer.SoftReset();
    barometer.Initialize();
  }
  else
  { 
    Serial.println("Could not connect to BMP180.");
  }

  /*-------- Configure 7-seg ----------*/
 
  int devices=lc.getDeviceCount();
 for(int address=0;address<devices;address++) {
    lc.shutdown(address,false);
    lc.setIntensity(address, 15);
    lc.clearDisplay(address);
  }

  /*-------- Begin WI-FI ----------*/
 
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
 
  /*-------- Begin ThingSpeak ----------*/
      
  ThingSpeak.begin(client);
  Serial.println("TimeNTP Example");
  
  /*-------- Wait until WI-FI starts ----------*/
  
  Serial.print("Connecting to "); 
  for(int x = 0; x <32; x++){
     Serial.print(ssid[x]);
  }
  Serial.println();
  
  bool stat = true;
  while (stat){
    parsing(); 
    if(WiFi.status() == WL_CONNECTED || millis() >= 25000){
      stat = false; 
    } 
  }
  
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("");
    Serial.println("WiFi connected"); 
  }else if(WiFi.status() != WL_CONNECTED){
    Serial.println("");
    Serial.println("WiFi NOT connected, conection timeout!"); 
  }

  /*-------- Start NTP ----------*/
  
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  printDigits();
  ThingSpeak.setStatus(password);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}

/*-------- Nuber Disasembler ----------*/

int numbdis(unsigned long input=0, int y = 0){
  int  x = 0;
  int  i = 0;
  if(input<10){
    i = 1;
  }else if(input<100){
    i = 2;
  }else if(input<1000){
    i = 3; 
  }else if(input<10000){
    i = 4;
  }else if(input<100000){
    i = 5;
  }else if(input<1000000){
    i = 6;
  }else if(input<10000000){
    i = 7;
  }else if(input<100000000){
    i = 8;
  }else if(input<1000000000){
    i = 9;
  }else{
    i = 10;
  }
    
  x = i-y;
  for(int a = x; a > 0; a--){      
    output=input%10;
    input/=10;         
}
    return output;
}

/*-------- Float multiplier function ----------*/

unsigned long floatmulplier(float input = 0){
  int multiplier;
  
  if(input >= 10 || input <= -10){
    multiplier = 10;
    dot = true;
  }else if(input < 10 && input > -10){
    multiplier = 100;
    dot = false;
  }
  
  if(input < 0){    
    negative = true; 
    return input *= -multiplier;
  }else{
    negative = false;
    return input *= multiplier;  
  }    
}

/*-------- Maesurments ----------*/

void masurement(){
  if(millis() - last_millis >= 2500){
    last_millis = millis();
    h = dht.readHumidity();
    temp = dht.readTemperature();    
  }
  
  if(barometer.IsConnected){
    p = barometer.GetPressure();
    p = (float)0.0075 * p;  
    pt = barometer.GetTemperature(); 
  } 
}

/*-------- Write data ti dislays ----------*/

void writer(){
  
  //#1 disolay
  
  if(temp < 1 && temp > -1){
    lc.setDigit(0,0,0,false); 
    lc.setDigit(0,1,numbdis(floatmulplier(temp),0),dot);
    lc.setDigit(0,2,0,!dot);     
  }else{
    lc.setDigit(0,0,numbdis(floatmulplier(temp),2),false); 
    lc.setDigit(0,1,numbdis(floatmulplier(temp),1),dot);
    lc.setDigit(0,2,numbdis(floatmulplier(temp),0),!dot);
  }
  
  if(negative){
    lc.setChar(0,3,'-',false);  
  }else{
    lc.setChar(0,3,' ',false);
  }
  
  lc.setDigit(0,4,numbdis(floatmulplier(h),2),false);
  lc.setDigit(0,5,numbdis(floatmulplier(h),1),dot);
  lc.setDigit(0,6,numbdis(floatmulplier(h),0),!dot);
    
  //#2 dislpay
 
  if(pt < 1 && pt > -1){
    lc.setDigit(1,0,0,false); 
    lc.setDigit(1,1,numbdis(floatmulplier(pt),0),dot);
    lc.setDigit(1,2,0,!dot);     
  }else{
    lc.setDigit(1,0,numbdis(floatmulplier(pt),2),false); 
    lc.setDigit(1,1,numbdis(floatmulplier(pt),1),dot);
    lc.setDigit(1,2,numbdis(floatmulplier(pt),0),!dot);
  }
  
  if(negative){
    lc.setChar(1,3,'-',false);  
  }else{
    lc.setChar(1,3,' ',false);
  }
  
  if(p < 1000){
    lc.setDigit(1,4,numbdis(floatmulplier(p),3),false); 
    lc.setDigit(1,5,numbdis(floatmulplier(p),2),true);
    lc.setDigit(1,6,numbdis(floatmulplier(p),1),false);
    lc.setDigit(1,7,numbdis(floatmulplier(p),0),false);
  }else{
    lc.setDigit(1,4,numbdis(floatmulplier(p),3),false); 
    lc.setDigit(1,5,numbdis(floatmulplier(p),2),false);
    lc.setDigit(1,6,numbdis(floatmulplier(p),1),false);
    lc.setDigit(1,7,numbdis(floatmulplier(p),0),false);
  }
  
  //#3 display
  
  lc.setChar(2,6,' ',false); 
  lc.setChar(2,7,' ',false);
  if(seconds > 9){
    lc.setDigit(2,0,numbdis(seconds,1),false); 
    lc.setDigit(2,1,numbdis(seconds,0),false);
  }else{
    lc.setDigit(2,0,seconds,false); 
    lc.setDigit(2,1,0,false);  
  } 
  
  if(minutes > 9){
    lc.setDigit(2,2,numbdis(minutes,1),blinker); 
    lc.setDigit(2,3,numbdis(minutes,0),false);
  }else{
    lc.setDigit(2,2,minutes,blinker); 
    lc.setDigit(2,3,0,false);  
  } 
  
  if(hours > 9){
    lc.setDigit(2,4,numbdis(hours,1),blinker); 
    lc.setDigit(2,5,numbdis(hours,0),false);
  }else{
    lc.setDigit(2,4,hours,blinker); 
    lc.setDigit(2,5,0,false);  
  }
  
 
  
}  

/*-------- Syncronization ----------*/
void online(){
  if(millis() - s_last_millis >= 30000){
    t = rtc.getTime();
    /*-------- display the date ----------*/
    lc.setDigit(2,0,numbdis(t.year,3),false); 
    lc.setDigit(2,1,numbdis(t.year,2),false);
    lc.setDigit(2,2,numbdis(t.year,1),false); 
    lc.setDigit(2,3,numbdis(t.year,0),false);
    if(t.mon > 9){
    lc.setDigit(2,4,numbdis(t.mon,1),true); 
    lc.setDigit(2,5,numbdis(t.mon,0),false);
  }else{
    lc.setDigit(2,4,t.mon,true); 
    lc.setDigit(2,5,0,false); 
  } 
  
  if(day() > 9){
    lc.setDigit(2,6,numbdis(t.date,1),true); 
    lc.setDigit(2,7,numbdis(t.date,0),false);
  }else{
    lc.setDigit(2,6,t.date,true); 
    lc.setDigit(2,7,0,false);   
  }
  /*-------- Syncronize the values ----------*/
  ThingSpeak.setField(1,temp);    
  ThingSpeak.setField(2,h); 
  ThingSpeak.setField(3,p);
  ThingSpeak.setField(4,pt);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(millis() - s_last_millis >= 33000){
     s_last_millis = millis(); 

  }   
 }  
}
/*-------- Time ----------*/
void timestuct(){

  
  t = rtc.getTime();
  minutes = t.min;
  hours = t.hour;
  seconds = t.sec;      
  value = analogRead(0);
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      writer();
      blinker = !blinker; 
      t = rtc.getTime();
      switch(syncinterval){
        case 0:
        if(t.hour == 0 && t.min == 0 && t.sec <= 30){
          syncronization();
        }
        break;
        case 1:
        if(t.hour == 0 && t.min == 0 && t.sec <= 30 || t.hour == 12 && t.min == 0 && t.sec <= 30){
          syncronization();
        }
        break;
        case 2:
         if(t.hour == 0 && t.min == 0 && t.sec <= 30 || t.hour == 12 && t.min == 0 && t.sec <= 30 || t.hour == 18 && t.min == 0 && t.sec <= 30 || t.hour == 6 && t.min == 0 && t.sec <= 30 ){
          syncronization();
        }
        break;
      }
    }
  }
}

/*-------- Print Digits ----------*/

void printDigits()
{
  Serial.print(hour());
  Serial.print(':');
  Serial.print(minute());
  Serial.print(':');
  Serial.print(second());
  Serial.print(' '); 
  Serial.print(day());
  Serial.print('/'); 
  Serial.print(month());
  Serial.print('/'); 
  Serial.println(year());  
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0); // discard any previously received packets
  Serial.println("Transmit NTP Request"); // get a random server from the pool
  WiFi.hostByName(serveradres, ntpServerIP);
  Serial.print(serveradres);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

/*-------- Serial parsing and writing to EEROM ----------*/

void parsing() {
  while (Serial.available() > 0) { //while there is data in serial buffer (128 bytes)
    char aChar = Serial.read();    //aChar - current symvol in buffer
    if (aChar != '#') {            //if aChar isnt the '#' (End) symbol do:
      inData[indexcount] = aChar;       //(inData - array, indexcount - loop № counter) write all Buffer to the array
      indexcount++;                     //indexcount + 1 - loop count
      inData[indexcount] = '\0';        //set the next position to '\0' or null symbol
    } else {                       //if aChar or current buffer symbol is '#' (End) do:
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
     EEPROM.commit();
    }
  }
}

/*-------- Read the EEPROM ----------*/

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
  if(gtmtime <= 15){
    timeZone = gtmtime;
     Serial.println(timeZone);
  }else{   
    timeZone = gtmtime - 15;
    timeZone = timeZone * -1;
    Serial.println(timeZone);
  }
  syncinterval = EEPROM.read(66);
  Serial.println(syncinterval);
  for(int i = 0; i < 100; i++){
    buffer[i] = EEPROM.read(i+67);
    Serial.print(buffer[i]);//67-167 ntpserver
    serveradres[i] = buffer[i];
  }
  Serial.println();
}

/*-------- Syncronize the clock ----------*/

void syncronization(){
  t = rtc.getTime();        
  rtc.setTime(hour(), minute(), second());
  rtc.setDate(day(), month(), year());
  dayofweek = (day() + 4) % 7 + 1;
  switch(dayofweek){
          case 1:
          rtc.setDOW(SUNDAY);
          break;
          case 2:
          rtc.setDOW(MONDAY);
          break; 
          case 3:
          rtc.setDOW(TUESDAY);
          break;
          case 4:
          rtc.setDOW(WEDNESDAY);
          break;
          case 5:
          rtc.setDOW(THURSDAY);
          break;
          case 6:
          rtc.setDOW(FRIDAY);
          break;
          case 7:
          rtc.setDOW(SATURDAY);
          break; 
  }         
}

/*-------- Loop all functions ----------*/


void loop() {
  parsing();
  timestuct();
  masurement();
  online();
  
  /*-------- Anti Overflow ----------*/
  if((unsigned long)millis() < (unsigned long)last_millis){
    ESP.reset();
  }
}



