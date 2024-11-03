#include <Wire.h>
#include "rgb_lcd.h"  

int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 7;//Variable for Soil moisture Power
const int pump = 3;
const int temp = A2;
const int fan = 13;
const int lightSens = A1; // Analog pin for the light sensor
const int led = 9;           // Digital pin for the LED

const float thresholdTemp = 24.0;
const int thresLight = 50;


// Define the threshold for light level
const int threshold = 50;
//Rather than powering the sensor through the 3.3V or 5V pins, 
//we'll use a digital pin to power the sensor. This will 
//prevent corrosion of the sensor as it sits in the soil. 
rgb_lcd lcd;

void setup() 
{
  Serial.begin(9600);
  //Temperature
  pinMode(temp, INPUT);
  pinMode(fan, OUTPUT);

  // LED
  pinMode(lightSens, INPUT);
  pinMode(led, OUTPUT);

  //water pump
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);

  //Soil Moisture
  pinMode(soilPower, OUTPUT);
  digitalWrite(soilPower, LOW);
  
  //LCD
  lcd.begin(16, 2); 
  lcd.setRGB(0, 128, 0);
}

void loop() 
{
int soilMoisture = readSoil();
int tC= readTemp();
Serial.print("Soil Moisture = ");    
//get soil moisture value from the function below and print it
Serial.println(soilMoisture);
if (soilMoisture<450){
  //  Serial.println("Soil moisture is not enough. Turning on pump.");
   digitalWrite(pump, HIGH);
   lcd.clear();
   lcd.setRGB(255, 0, 0);  
   lcd.setCursor(0, 0);
   lcd.print("Water Level:");
   lcd.print(soilMoisture/180, 1);

   
   lcd.setCursor(0, 1);
   lcd.print("Dry! Pump is on");
}else {
    // Serial.println("Soil moisture is adequate. Turning off pump.");
    digitalWrite(pump, LOW);
    lcd.clear();
    lcd.setRGB(0, 128, 0);
    lcd.setCursor(0, 0);
    lcd.print("Water Level:");
    lcd.print(soilMoisture/180, 1);

   
    lcd.setCursor(0, 1);
    lcd.print("Wet! Pump is off.");   // Turn off pump
  }

//This 1 second timefrme is used so you can test the sensor and see it change in real-time.
//For in-plant applications, you will want to take readings much less frequently.
delay(1500);

Serial.print("Temperature: ");
Serial.print(tC);
Serial.print(" C ");
lcd.clear();

lcd.setCursor(0, 0);               // Set cursor to top left
lcd.print("Temperatue: ");
lcd.print(tC);
lcd.print("C");
  if (tC > thresholdTemp) {
      digitalWrite(fan, HIGH);    // Turn on the  fan
      lcd.setRGB(255, 0, 0);
      lcd.setCursor(0, 1);
      lcd.print("Fan turned on");
               // Change backlight to red if temperature is high
  } else {
        digitalWrite(fan, LOW);     // Turn off the fan
        lcd.setRGB(0, 128, 0);
        lcd.setCursor(0, 1);
        lcd.print("Fan turned off");       // Change backlight to blue if temperature is normal
    }
    
    delay(1500); //take a reading every second

    int lightIntensity = analogRead(lightSens);
    Serial.print("Light Intensity: ");
    Serial.println(lightIntensity);
    lcd.clear();
    lcd.setCursor(0, 0);               // Set cursor to top left
    lcd.print("Light Intensity: ");
    lcd.setCursor(0, 1);
    lcd.print(lightIntensity);
    if (lightIntensity < thresLight) {
    // If it's dark, turn on the LED
    digitalWrite(led, HIGH);
    lcd.setRGB(128, 128, 128);

  } else {
    // If it's bright, turn off the LED
    digitalWrite(led, LOW);
    
  }
  delay(1500);
}
//This is a function used to get the soil moisture content
int readSoil()
{

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}
int readTemp()
{
   float tv = analogRead(temp);
   float tmv = (tv * 5000) / 1024; 
   float tc = ((tmv - 500) / 10)*0.1;    
   float tcc = (tmv - 500) / 10; 
   return tc;
}