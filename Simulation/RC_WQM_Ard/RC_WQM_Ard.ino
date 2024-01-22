#include <Adafruit_GFX.h>
#include <SimpleTimer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "GravityTDS.h"
#include <DallasTemperature.h>
#include <TinyGPS++.h>
#define SAMPLES 300
#include<SoftwareSerial.h>
#include <ArduinoJson.h>

SimpleTimer timer;
LiquidCrystal_I2C lcd(0x20, 16, 2);
SoftwareSerial nodemcu(10, 11); 

//////////////////////GPS//////////////////////////
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//////////////////////PH//////////////////////////
#define SensorPin A2
unsigned long int avgValue;
float b;
int buf[10], temp;

///////////////////////////////////turbidity ///////////////////////////////////
int Tur = A0;

///////////////////////////Temperature/////////////////////////////////
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

///-----------------------Voltage-------------------
#define Vol_V A3
double V = 0;
int offset = 18.7;
float percn = 0;
float C_mil = 0;

////--------------------State----------------------
int green = 8;
int red = 7;

////////////Data Transfer///////////////////////////////
float percn_N, V_N,turbidity_N, TepmA_N, phValue_N;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  nodemcu.begin(9600);
  ss.begin(GPSBaud);

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    !!SHA!! ");
  //Serial.println("");


  pinMode(7,OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Apartment 1");
  lcd.setCursor(1, 01);
  lcd.print("Temp:");
  lcd.setCursor(7, 1);
  lcd.print("26.02 'C");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Apartment 1");
  lcd.setCursor(1, 01);
  lcd.print("Fire:");
  lcd.setCursor(7, 1);
  lcd.print("Nagetive");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Apartment 2");
  lcd.setCursor(1, 01);
  lcd.print("Temp:");
  lcd.setCursor(7, 1);
  lcd.print("27.00 'C");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Apartment 2");
  lcd.setCursor(1, 01);
  lcd.print("Fire:");
  lcd.setCursor(7, 1);
  lcd.print("Nagetive");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Apartment 3");
  lcd.setCursor(1, 01);
  lcd.print("Temp:");
  lcd.setCursor(7, 1);
  lcd.print("25.00 'C");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Apartment 3");
  lcd.setCursor(1, 01);
  lcd.print("Fire:");
  lcd.setCursor(7, 1);
  lcd.print("Nagetive");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Apartment 4");
  lcd.setCursor(1, 01);
  lcd.print("Temp:");
  lcd.setCursor(7, 1);
  lcd.print("26.00 'C");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Apartment 4");
  lcd.setCursor(1, 01);
  lcd.print("Fire:");
  lcd.setCursor(7, 1);
  lcd.print("Positive");
  delay(1000);

  Serial.println("Apartment 1");
  Serial.print("Temp:");
  Serial.println("26.02 'C");
  Serial.print("Fire: ");
  Serial.println("Nagetive");

  Serial.println("Apartment 2");
  Serial.print("Temp:");
  Serial.println("27.00 'C");
  Serial.print("Fire: ");
  Serial.println("Nagetive");

  Serial.println("Apartment 3");
  Serial.print("Temp:");
  Serial.println("25.00 'C");
  Serial.print("Fire: ");
  Serial.println("Nagetive");

  Serial.println("Apartment 4");
  Serial.print("Temp:");
  Serial.println("26.00 'C");
  Serial.print("Fire: ");
  Serial.println("Positive");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ADMIN ");
  lcd.setCursor(1, 01);
  lcd.print("Gas:");
  lcd.setCursor(7, 1);
  lcd.print("Not Detect");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ADMIN ");
  lcd.setCursor(1, 01);
  lcd.print("Lift:");
  lcd.setCursor(7, 1);
  lcd.print("ON");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ADMIN ");
  lcd.setCursor(1, 01);
  lcd.print("Water Pump:");
  lcd.setCursor(11, 1);
  lcd.print("ON");
  delay(1000);

    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ADMIN ");
  lcd.setCursor(1, 01);
  lcd.print("Water Level:");
  lcd.setCursor(11, 1);
  lcd.print("106 cm");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ADMIN ");
  lcd.setCursor(1, 01);
  lcd.print("Car:");
  lcd.setCursor(11, 1);
  lcd.print("Parked");
  delay(1000);

  Serial.println("ADMIN");
  Serial.print("Gas: ");
  Serial.println("Not Detec");
  Serial.print("Lift: ");
  Serial.println("ON");
  Serial.print("Water Pump: ");
  Serial.println("ON");
  Serial.print("Water Level: ");
  Serial.println("106 cm");
  Serial.print("Car: ");
  Serial.println("Parked");

//  //-------------------------------Voltage-------------------------------------------------
//  int volt = analogRead(Vol_V);
//  V = map(volt, 0, 1023, 0, 2200);
//
//  V /= 100;
//
//  //-----------------------------percentage----------------------------------------------
//  percn = (V / 10) * 100;
//
//  /////////////////////////pH Calclution///////////////
//  for (int i = 0; i < 10; i++)
//  {
//    buf[i] = analogRead(SensorPin);
//    delay(10);
//  }
//  for (int i = 0; i < 9; i++)
//  {
//    for (int j = i + 1; j < 10; j++)
//    {
//      if (buf[i] > buf[j])
//      {
//        temp = buf[i];
//        buf[i] = buf[j];
//        buf[j] = temp;
//      }
//    }
//  }
//  avgValue = 0;
//  for (int i = 2; i < 8; i++)
//    avgValue += buf[i];
//  float phValue = (float)avgValue * 5.0 / 1024 / 6;
//  phValue = 6.7;
////
////random(6.2, 7.5);
//float ran = 6.9;
////Serial.println("PHH: ");
////Serial.print(ran);
//
//
//  ///////////////////////////turbidity ////////////////
//
//  int sensorValue = analogRead(Tur);
//  int turbidity = 18;
//  
//  delay(100);
//
//  ////////////////////////////////TEmperature/////////////////////////
//  sensors.requestTemperatures();
//  float tempA = 32;
//
//
//
//  //////////////////////////////////Display/////////////////////////////////
//
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("     !!WVS!!");
//  delay(2000);
//
//  if (gps.date.isValid())
//  {
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("     Date:");
//    lcd.setCursor(3, 1);
//    lcd.print(gps.date.month());
//    lcd.print(F("/"));
//    lcd.print(gps.date.day());
//    lcd.print(F("/"));
//    lcd.print(gps.date.year());
//    delay(2000);
//  }
//
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("    Voltage  ");
//  lcd.setCursor(5, 1);
//  lcd.print("8.35");
//  lcd.setCursor(9, 1);
//  lcd.print("V");
//  delay(2000);
//
//  Serial.println("Voltage");
//  Serial.print(V);
//
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("  Battery Perc  ");
//  lcd.setCursor(5, 1);
//  lcd.print("83.50");
//  lcd.setCursor(9, 1);
//  lcd.print("%");
//  delay(2000);
//  Serial.println("Battery Perc");
//  Serial.print(percn);
//
//
//  Serial.print("    PH:");
//  Serial.print(phValue, 2);
//  Serial.println(" ");
//
//  if (tempA > 15 && tempA < 26 ) {
//    phValue = 6.4;
//    digitalWrite(red, HIGH);
//    digitalWrite(green, LOW);
//  }
//  else if (tempA > 26 && tempA < 30)
//  {
//    phValue = 6.7;
//    digitalWrite(red, LOW);
//    digitalWrite(green, HIGH);
//  }
//  else
//  {
//    phValue = 7.3;
//    digitalWrite(red, LOW);
//    digitalWrite(green, HIGH);
//  }
//  
//  
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("PH:");
//  lcd.setCursor(4, 0);
//  lcd.print(phValue);
//  
//  if (phValue < 6 ) {
//    lcd.setCursor(0, 1);
//    lcd.print("PH: Acid");
//  }
//  else if (phValue > 8)
//  {
//    lcd.setCursor(0, 1);
//    lcd.print("PH: Alkine");
//  }
//  else
//  {
//    lcd.setCursor(0, 1);
//    lcd.print("PH: Normal");
//  }
//  delay(2000);
//
//                        Serial.println("turbidity Value: ");
//                        Serial.print(turbidity);
//                        Serial.println("ppm");
//
// 
//    
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Turbidity:");
//    lcd.setCursor(11, 0);
//    lcd.print(turbidity);
//     
//   if (turbidity < 20) {
//    digitalWrite(7, HIGH);
//    digitalWrite(8, LOW);
//    digitalWrite(9, LOW);
//    lcd.setCursor(0, 1);
//    lcd.print("State    : CLEAR ");
//  }
//  if ((turbidity > 10) && (turbidity < 50)) {
//    digitalWrite(7, LOW);
//    digitalWrite(8, HIGH);
//    digitalWrite(9, LOW);
//    lcd.setCursor(0, 1);
//    lcd.print("State   : CLOUDY ");
//  }
//  if (turbidity > 50) {
//    digitalWrite(7, LOW);
//    digitalWrite(8, LOW);
//    digitalWrite(9, HIGH);
//    lcd.setCursor(0, 1);
//    lcd.print("State    : DIRTY ");
//  }
//    delay(2000);
//
//
//  Serial.print("Temperature for the device 1 (index 0) is: ");
//  Serial.println(tempA);
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("W. Temp:");
//  // lcd.setCursor(0, 1);
//  lcd.print(tempA);
//  lcd.print(" `C");
//  if (tempA < 20 ) {
//    lcd.setCursor(0, 1);
//    lcd.print("State  : Cold");
//  }
//  else if (tempA > 20 & tempA < 33)
//  {
//    lcd.setCursor(0, 1);
//    lcd.print("State  : Normal");
//  }
//  else
//  {
//    lcd.setCursor(0, 1);
//    lcd.print("State  : Hot");
//  }
//  delay(2000);
//
//  
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("Latitude :");
//  lcd.setCursor(11, 0);
//  lcd.print("23.8103");
//  lcd.setCursor(0, 1);
//  lcd.print("Longitude:");
//  lcd.setCursor(11, 1);
//  lcd.print("90.4125");
//  delay(2000);


}
