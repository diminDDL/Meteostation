   //We always have to include the library
#include <LedControlMS.h>
#include <DS1307.h>
#include "ThingSpeak.h"
#include <DHT.h>
#include <Wire.h>
#include <BMP180.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

int dayofweek;
extern "C" {
    #include "user_interface.h"
}
#define DHTPIN 13     
#define DHTTYPE DHT22 
BMP180 barometer;
DHT dht(DHTPIN, DHTTYPE);
DS1307  rtc(12,  14);//scl D5  sda D6
Time  t;
//Adafruit_BMP085 bmp;

unsigned long last_millis;
unsigned long s_last_millis;
float h;
float temp;
float p;
float pt;
boolean negative = false;
boolean dot;
boolean blinker = true;
int newmin = t.min;
int newhour = t.hour;
int minutes = t.min;
int hours = t.hour;
int seconds = t.sec;
int value;
unsigned long inpu = 0;//0 to 4,294,967,295
int output = 0;
int number = 0;


static const char ntpServerName[] = "ntp.time.in.ua";
const int timeZone = 2;
WiFiUDP Udp;
unsigned int localPort = 8888;
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);



#if defined(ARDUINO_AVR_YUN)
    #include "YunClient.h"
    YunClient client;
#else
  #if defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_ARCH_ESP8266)
    // Use WiFi
    #ifdef ARDUINO_ARCH_ESP8266
      #include <ESP8266WiFi.h>
    #else
      #include <SPI.h>
      #include <WiFi101.h>
    #endif
    char ssid[] = "";    //  your network SSID (name) 
    char pass[] = "";   // your network password
    int status = WL_IDLE_STATUS;
    WiFiClient  client;
  #elif defined(USE_ETHERNET_SHIELD)
    // Use wired ethernet shield
    #include <SPI.h>
    #include <Ethernet.h>
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    EthernetClient client;
  #endif
#endif


/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(0,2,15,3);

unsigned long myChannelNumber = 0;// your ID
const char * myWriteAPIKey = "";// your API

void setup() {
  WiFi.mode(WIFI_STA);

  rtc.halt(false);

  Wire.begin();
  barometer = BMP180();
   
  Serial.begin(9600);
  if(barometer.EnsureConnected())
  {
    Serial.println("Connected to BMP180."); // Output we are connected to the computer.    
     // When we have connected, we reset the device to ensure a clean start.
    barometer.SoftReset();
    // Now we initialize the sensor and pull the calibration data.
    barometer.Initialize();
  }
  else
  { 
    Serial.println("Could not connect to BMP180.");
  }
  
  dht.begin();
  int devices=lc.getDeviceCount();
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
 for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,8);
    /* and clear the display */
    lc.clearDisplay(address);
  }
   #ifdef ARDUINO_AVR_YUN
    Bridge.begin();
  #else   
    #if defined(ARDUINO_ARCH_ESP8266) || defined(USE_WIFI101_SHIELD) || defined(ARDUINO_SAMD_MKR1000)
      WiFi.begin(ssid, pass);
    #else
      Ethernet.begin(mac);
    #endif
  #endif

  ThingSpeak.begin(client);
  Serial.println("TimeNTP Example");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wi_fi_timeout = 0;
  bool stat = true;
  while (stat){
    delay(500);
    wi_fi_timeout++;
    Serial.print("."); 
    if(WiFi.status() == WL_CONNECTED || wi_fi_timeout == 50){
      stat = false;
    } 
  }
 // while (WiFi.status() != WL_CONNECTED) {
    

 // }
    
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  Serial.println("LAST SETUP COMAND");
}
time_t prevDisplay = 0;


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

void masurement(){

  if(millis() - last_millis >= 2000){
    last_millis = millis();
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp = dht.readTemperature();
    blinker = !blinker;
  }
  if(barometer.IsConnected)
  {
    p = barometer.GetPressure();
    p = (float)0.0075 * p;  
    pt = barometer.GetTemperature(); 
  }
  
 
  
  
  
}
void writer(){
  //#1
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
  //#2
  delay(10);
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
  //#3
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


void online(){
  if(millis() - s_last_millis >= 30000){
      t = rtc.getTime();
     ////////////////-------////////////
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
     ThingSpeak.setField(1,temp);    
     ThingSpeak.setField(2,h); 
     ThingSpeak.setField(3,p);
     ThingSpeak.setField(4,pt);
    // ThingSpeak.setStatus("updated! ");
     ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
     if(millis() - s_last_millis >= 33000){
     s_last_millis = millis(); 

     }
     

 }  
}
void timestuct(){

  t = rtc.getTime();
  newmin = t.min;
  newhour = t.hour;
  minutes = t.min;
  hours = t.hour;
  seconds = t.sec;
  value = analogRead(0);
 
   
if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
       t = rtc.getTime();
      digitalClockDisplay();
      if(t.hour == 0 && t.min == 0 && t.sec <= 30 || digitalRead(16) == HIGH){
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
      }else{
        
      }
    }
  }

}
void loop() {
 
  timestuct();
  masurement();
  writer();
  online();
}
void digitalClockDisplay()
{

  
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
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



