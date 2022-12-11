#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define REPORTING_PERIOD_MS     1000
 
PulseOximeter pox;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    Serial.println("Beat!");
}
 
void setup()
{
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);

     if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("   HEART BEAT!!!");
  display.display(); 
}
 float heart_beat = 0.00;
 float Sp02 = 0.00;
void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
    display.clearDisplay();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        // heart_beat = pox.getHeartRate();
        // Sp02 = pox.getSpO2();    
           
        // Serial.print("Heart rate:");
        // Serial.print(pox.getHeartRate());
        // Serial.print("bpm / SpO2:");
        // Serial.print(pox.getSpO2());
        // //Serial.print(pox.getSpO2());
        // Serial.println("%");

        display.setTextSize(1); 
        display.setCursor(0, 0);
        // Display static text
        display.println("Heart rate:");
        display.setTextSize(2);         
        display.setCursor(50, 8);        
        display.print(pox.getHeartRate());
        display.setCursor(0, 32);
        // Display static text
         display.setTextSize(1);  
        display.println("SpO2:");
        display.setCursor(50, 40);    
        display.setTextSize(2);             
        display.print(pox.getSpO2());
        //display.print("%");        
        display.display();     
         
        tsLastReport = millis();
    }
}