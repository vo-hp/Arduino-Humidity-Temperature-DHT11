#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 10

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool wasBlink = false;

void lcdBlink() {
  for (int i = 0; i < 10; i++) {
    lcd.noBacklight();
    delay(100);    
    lcd.backlight();
    delay(100);
  }  
}

void setup(){
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
}

void loop(){
  DHT.read(DHT11_PIN);
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
  if (DHT.humidity >= 80) {
    lcd.setCursor(0, 0);
    lcd.print("warning");
    if (not wasBlink) {
      lcdBlink();
      wasBlink = true;
    }  
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("       ");
    wasBlink = false;
  }
  delay(1000);
}