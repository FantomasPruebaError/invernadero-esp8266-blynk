/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"

#define DEBUG(a) Serial.println(a);

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "WalmefbFWbBE0nBrPru72CntRFskSWlH";

#define DHTPIN D4          // What digital pin we're connected to
#define Act_deshum D5
#define Act_luz D6
#define Act_bba D7
#define Act_vent D2
#define Act_timer D3
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int val;
int sel;
int bba_manual;
int luz_manual;
int luz_timer;

float h;
float t;
float tr;
int ht;
int sensor_pin;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Ramona";
char pass[] = "81239819";

//Timer Lights
BLYNK_WRITE(V0)
{
    luz_timer = param.asInt();
}

// Auto/Manual Mode switch
BLYNK_WRITE(V1)
{
    sel = param.asInt();
}

BLYNK_WRITE(V2)
{
    bba_manual = param.asInt(); // assigning incoming value from pin V1 to a variable
  // process received value
}

BLYNK_WRITE(V8)
{
    luz_manual = param.asInt(); // assigning incoming value from pin V1 to a variable
  // process received value
}

void sendSensor()
{
  sensor_pin = A0;
  ht = 0;
  h = dht.readHumidity();
  t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  tr = t + (35*(log10(h/100)));
  ht = analogRead(sensor_pin);
  ht = map(ht,750,180,0,100); 
  
  if (isnan(h) || isnan(t)){
    Serial.println("Failed to read from DHT/Soil Moisture sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, ht);
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, tr);
}
 
void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(Act_luz, OUTPUT);
  pinMode(Act_bba, OUTPUT);
  pinMode(Act_deshum, OUTPUT);
  pinMode(Act_vent, OUTPUT);
  pinMode(Act_timer, OUTPUT);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize Blynk 
  Blynk.begin(auth, ssid, pass);

  // Initialize DHT sensor
  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

}
   
void loop(){
  
  Blynk.run();
  timer.run();

  if(sel == 255){
          
          if(bba_manual == 255){// Activa la bomba en forma manual mediante bóton Blynk
            digitalWrite(Act_bba, HIGH); Blynk.virtualWrite(V9, 255);    
          }
          else{
            digitalWrite(Act_bba, LOW); Blynk.virtualWrite(V9, 0);  
          }
        
         
          if(luz_manual == 255){ // Activa la luz en forma manual
            digitalWrite(Act_luz, HIGH); Blynk.virtualWrite(V10, 255);    
          }
          else{
            digitalWrite(Act_luz, LOW); Blynk.virtualWrite(V10, 0); 
          }
    
   } else {
          
          if(luz_timer == 255){       
                digitalWrite(Act_timer, HIGH); Blynk.virtualWrite(V13,255); 
            } else {
                digitalWrite(Act_timer, LOW);  Blynk.virtualWrite(V13,0);
            }
        
          if(h >= 55){
                digitalWrite(Act_deshum, HIGH); Blynk.virtualWrite(V12,255);
            } else {
                digitalWrite(Act_deshum, LOW);  Blynk.virtualWrite(V12,0);
          }
        
          if(t >= 15){
                digitalWrite(Act_vent, HIGH); Blynk.virtualWrite(V11,255);
            } else {
                digitalWrite(Act_vent, LOW);  Blynk.virtualWrite(V11,0);
          }
        
          }
  
}
