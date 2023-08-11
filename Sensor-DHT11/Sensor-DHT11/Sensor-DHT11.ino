#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 10

LiquidCrystal_I2C lcd(0x27, 16, 2);


const unsigned long INTERVAL_READSENSOR = 1000;
unsigned long currentMillisReadSensor;
unsigned long previousMillisReadSensor = 0;

const unsigned long INTERVAL_BLINKLCD = 300;
unsigned long previousMillisBlinkLcd = 0;
unsigned long currentMillisBlinkLcd;


const int HUMI_THRESHOLD = 80;
bool isHumiHigherThanThreshold = false;
bool isLcdBlink = false;
bool wasLcdOn = false;
bool isSensorValueChanged = false;
bool wasLcdWarning = false;

void readSensor() {
  currentMillisReadSensor = millis();
  if (currentMillisReadSensor - previousMillisReadSensor >= INTERVAL_READSENSOR) {
    previousMillisReadSensor = millis();
    DHT.read(DHT11_PIN);
    isSensorValueChanged = true;
    Serial.print("temp: ");
    Serial.println(DHT.temperature);
    Serial.print("humi: ");
    Serial.println(DHT.humidity);
  }
}

void printLcd() {
  if (isSensorValueChanged) {
    lcd.setCursor(0, 1);
    lcd.print("temp:");
    lcd.setCursor(5, 1);
    lcd.print(DHT.temperature);
    lcd.setCursor(8, 1);
    lcd.print("humi:");
    lcd.setCursor(13, 1);
    lcd.print(DHT.humidity);
    isSensorValueChanged = false;
  }
}

void checkHumidity() {
  if (DHT.humidity >= HUMI_THRESHOLD) {
    lcdPrintWarning();
    blinkLcd();
  } else {
    if (wasLcdWarning) {
      lcdClearWarning();
    }
    if (not wasLcdOn) {
      lcd.backlight();
    }
  }
}

void blinkLcd() {
  currentMillisBlinkLcd = millis();
  if (currentMillisBlinkLcd - previousMillisBlinkLcd >= INTERVAL_BLINKLCD) {
    previousMillisBlinkLcd = currentMillisBlinkLcd;
    if (wasLcdOn) {
      lcd.noBacklight();
      wasLcdOn = false;
      Serial.println("tat");
    } else {
      lcd.backlight();
      wasLcdOn = true;
      Serial.println("bat");
    }
  }
}


void lcdPrintWarning() {
  if (not wasLcdWarning) {
    lcd.setCursor(0, 0);
    lcd.print("warning");
    wasLcdWarning = true;
  }
}

void lcdClearWarning() {
  if (wasLcdWarning) {
    lcd.setCursor(0, 0);
    lcd.print("       ");
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
}

void loop() {
  readSensor();
  printLcd();
  checkHumidity();
}