
#define BLYNK_PRINT Serial  
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <BlynkSimpleLinkItONE.h>
#include <SPI.h>
#include <SimpleTimer.h>
/// DS18B20 library
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h> 

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "116fb9ba4e334d8082a404032d8745b3";
SimpleTimer timer;
float temp =0;
int relayV1=0;
int relayV2=0;
int relayV3=0;
int relayV6=0;
int slider=0;
int food=0;

int FoodAngle=0;
int num1=0;
int num2=0;
int hour=0;
int PumpSchedule=0;

int pinLight = A0;
Servo myservo;

#define WIFI_SSID "HSCC"     // Your network SSID (name)
#define ARDUINO
#define WIFI_PASS "ilovehscc"     // Your network password (use for WPA, or use as key for WEP)
#define WIFI_AUTH LWIFI_WPA      // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP
// code for DS18B20
#define ONE_WIRE_BUS 2        // D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
/////////////////////////

void sendUptime()
{
sensors.requestTemperatures(); // Send the command to get temperatures
//Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"?
//delay(500);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
 float t = roundf(sensors.getTempCByIndex(0) * 100) / 100;
if(t!=-127) temp=t;
Blynk.virtualWrite(V5, temp);
Serial.println(temp); // Why "byIndex"?
}
BLYNK_WRITE(V0)
{
  // Get the state of the VButton
  relayV1 = param.asInt();
  
}

BLYNK_WRITE(V1)
{
  // Get the state of the VButton
  relayV2 = param.asInt();
  
}
BLYNK_WRITE(V2)
{
  // Get the state of the VButton
  relayV3 = param.asInt();
  
}
BLYNK_WRITE(V6)
{
  // Get the state of the VButton
  relayV6 = param.asInt();
  
}
BLYNK_WRITE(V3)
{
  // Get the state of the VButton
  slider = param.asInt();
  
}
void setup()
{
  sensors.begin();// DS18B20 sensor
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(7, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(auth, WIFI_SSID, WIFI_PASS, WIFI_AUTH);
  timer.setInterval(1000L, sendUptime);

    myservo.attach(3);
    myservo.write(90);
  
}
void AutoControl(){
    int light_value = analogRead(pinLight);
    if(light_value<700) digitalWrite(7, HIGH);
    else digitalWrite(7, LOW);
    Serial.println("light: ");
   Serial.println(light_value);
   Serial.println("time1: ");
   Serial.println(num1); 
   Serial.println("time2: ");
   Serial.println(num2); 
    if(temp<20) digitalWrite(5, HIGH);
    if(temp>30) digitalWrite(5, LOW);

    delay(1000);
    num1++;
    num2++;
    
    if (num1==21600){
      FoodAngle+=18;
    if(FoodAngle<180)
    myservo.write(FoodAngle);
    else FoodAngle=0;
    num1=0;
   }
   
    if(num2==3600)digitalWrite(6, HIGH);
    if (num2==7200) {
      digitalWrite(6, LOW);
      num2=0;
    }
   

    
    
}
void ManualControl(){
    if(relayV1==0)  digitalWrite(5, LOW);
    else digitalWrite(5, HIGH);
    
    if(relayV2==0)  digitalWrite(6, LOW);
    else digitalWrite(6, HIGH);
    
    if(relayV3==0)digitalWrite(7, LOW);
    else digitalWrite(7, HIGH);

    //map(slider,0,100,0,360);
    int angle=slider*1.79;
   // if(angle!=food){
   /// if()
    myservo.write(angle);
    //angle=slider;
   // }
}

void loop()
{
 //Serial.println("Connecting to AP");
// Serial.println(value);
 //delay(1000);
  if(relayV6==0) ManualControl();
  else AutoControl();

    
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
 // delay(1000);
}
