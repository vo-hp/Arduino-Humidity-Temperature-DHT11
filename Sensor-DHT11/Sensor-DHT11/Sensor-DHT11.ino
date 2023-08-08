#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 10

LiquidCrystal_I2C lcd(0x27, 16, 2);

const long interval = 1000;
unsigned long previousReadMillis = 0;
unsigned long previousLcdMillis = 0;
unsigned long previousBlinkMillis = 0;
unsigned long currentMillis;

bool wasBlink = false;
int blinkCount = 0;
int a = 0;


void read() {
  lcd.setCursor(0, 1);
  lcd.print("temp:");
  lcd.setCursor(5, 1);
  lcd.print(DHT.temperature);
  lcd.setCursor(8, 1);
  lcd.print("humi:");
  lcd.setCursor(13, 1);
  lcd.print(DHT.humidity);
  Serial.print("temp: ");
  Serial.println(DHT.temperature);
  Serial.print("humi: ");
  Serial.println(DHT.humidity);
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousReadMillis >= interval) {
    previousReadMillis = millis();
    DHT.read(DHT11_PIN);
    read();
  }
  if (currentMillis - previousLcdMillis >= 100) {
    previousLcdMillis = millis();
    if (DHT.humidity >= 90) {
      lcd.setCursor(0, 0);
      lcd.print("warning");
      if (not wasBlink) {
        if (blinkCount <= 8) {
          if (a == 0) {
            lcd.noBacklight();
            a = 1;
            Serial.println("tat");
          } else {
            lcd.backlight();
            a = 0;
            Serial.println("bat");
          }
          blinkCount++;
          if (blinkCount == 8) {
            wasBlink = true;
          }
        }
      }  
    }  
    else {
      lcd.setCursor(0, 0);
      lcd.print("       ");
      wasBlink = false; 
      blinkCount = 0;
    }
  }
}