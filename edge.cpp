#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Wire.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);  

const int tempSensorPin = A1;aa
const int lightSensorPin = A0;
const int redLedPin = 11;
const int buzzerPin = 10;
const int luminosityLedPins[] = {0,1,2,3,4,5}; 
const int anomalyThreshold = 23; 
const int readingsToAverage = 30;

int tempReadings[readingsToAverage];
int lightReadings[readingsToAverage];
int tempIndex = 0;
int lightIndex = 0;


void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < sizeof(luminosityLedPins)/sizeof(luminosityLedPins[0]); i++) {
    pinMode(luminosityLedPins[i], OUTPUT);
  }
}

void loop() {
  float tempCelsius = readTemperature();
  int lightPercentage = readLight();

  tempReadings[tempIndex] = tempCelsius;
  lightReadings[lightIndex] = lightPercentage;
  tempIndex = (tempIndex + 1) % readingsToAverage;
  lightIndex = (lightIndex + 1) % readingsToAverage;

  if (tempCelsius > anomalyThreshold) {
    digitalWrite(redLedPin, HIGH);
    tone(buzzerPin, 1000);
    delay(200);
    noTone(buzzerPin);
    
    int address = findNextAvailableAddress();
    EEPROM.write(address, round(calcAverage(tempReadings, readingsToAverage)));
    EEPROM.write(address + 1, round(calcAverage(lightReadings, readingsToAverage)));
  } else {
    digitalWrite(redLedPin, LOW);
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempCelsius);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Light: ");
  lcd.print(lightPercentage);
  lcd.print("% ");
  
  lcd.setCursor(10, 1); 
  lcd.print("B");

  lcd.setCursor(11, 1); 
  lcd.print("S");
  

  updateLuminosityGraph(lightPercentage);
  
  delay(2000); 
}

float readTemperature() {
  return analogRead(tempSensorPin) * 0.48828125; // Converte valor para temperatura em °C
}

int readLight() {
  int lightValue = analogRead(lightSensorPin);
  int lightPercentage = map(lightValue, 0, 675, 0, 100); // Mapeia para 0-100%
  return lightPercentage;
}

float calcAverage(int * readings, int count) {
  float sum = 0;
  for (int i = 0; i < count; i++) {
    sum += readings[i];
  }
  return sum / count;
}

void updateLuminosityGraph(int percentage) {
  int numLedsToShow = map(percentage, 0, 100, 0, sizeof(luminosityLedPins)/sizeof(luminosityLedPins[0]));
  
  for (int i = 0; i < sizeof(luminosityLedPins)/sizeof(luminosityLedPins[0]); i++) {
    digitalWrite(luminosityLedPins[i], i < numLedsToShow ? HIGH : LOW);
  }
}

int findNextAvailableAddress() {
  for (int i = 0; i < 900; i += 2) { 
    if (EEPROM.read(i) == 0 && EEPROM.read(i + 1) == 0) {
      return i;
    }
  }
  return -1; 
}
