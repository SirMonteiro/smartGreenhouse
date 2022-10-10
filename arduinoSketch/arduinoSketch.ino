#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
// ports
#define SOILHUMIDITYSENSORPIN A7
#define LIGHTSENSORPIN A0
#define RELAYPIN 2
#define DHTSENSORPIN 22

String readString;
boolean pumpRelay = false;
int textLCDIndex;

DHT dht(DHTSENSORPIN, DHT22);
LiquidCrystal_I2C lcd(0x27,16,2);

byte happyFace[8]={
B00000,
B11011,
B11011,
B00000,
B10001,
B01110,
B01110,
B00000
};

byte sadFace[8]={
B00000,
B11011,
B11011,
B00000,
B01110,
B01110,
B10001,
B00000
};

byte acento[8]={
B00010,
B00100,
B01110,
B00001,
B01111,
B10001,
B01111,
B00000
};

void setup() {
  // USB Serial
  Serial.begin(115200);
  Serial.println("Serial opened!");

  // Bluetooth serial
  Serial1.begin(38400);

  // DHT
  dht.begin();

  // LCD
  lcd.init();
  lcd.createChar(5, happyFace);
  lcd.createChar(6, sadFace);
  lcd.createChar(1, acento);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("    Horta");
  lcd.setCursor(0,1);
  lcd.print(" carregando...");

  pinMode(LIGHTSENSORPIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);

  delay(500);
}

void loop() {

  while (Serial1.available()) {
    delay(3); 
    char c = Serial1.read();
    readString += c;
  }

  float airTemperature = dht.readTemperature();
  float airHumidity = dht.readHumidity();
  int soilHumidity = analogRead(SOILHUMIDITYSENSORPIN);
  int soilHumidityPercentage = map(analogRead(SOILHUMIDITYSENSORPIN), 0, 1023, 0, 100);
  int lightPercentage = map(analogRead(LIGHTSENSORPIN), 0, 1023, 0, 100);

  if (lightPercentage <= 10){
    lightPercentage = 0;
  }
  
  if (lightPercentage > 25) {
    lightPercentage += 30;
  }

  if (lightPercentage > 100) {
    lightPercentage = 100;
  }

  if (soilHumidity > 700) {
    digitalWrite(RELAYPIN, HIGH);
    pumpRelay = true;
    if (textLCDIndex != 1) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Humidade baixa!");
      lcd.write(6);
      lcd.setCursor(0,1);
      lcd.print("Regando...");
      textLCDIndex = 1;
    }
    
  } else {
    digitalWrite(RELAYPIN, LOW);
    pumpRelay = false;
    if (textLCDIndex != 2) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Planta saud");
    lcd.write(1);
    lcd.print("vel");
    lcd.write(5);
    lcd.setCursor(0,1);
    lcd.print("Estado ideal");
    textLCDIndex = 2;
    }
  }

  if (readString.length() > 0) {
    if (readString == "send") {
      Serial1.println('~');
      Serial1.println(airTemperature, 1);
      Serial1.println(airHumidity, 0);
      Serial1.println(100 - soilHumidityPercentage);
      Serial1.println(lightPercentage);
      Serial1.println(pumpRelay ? '1' : '0');
      Serial1.println('*');
    }
    readString = "";
  }

  // Serial.println('~');
  // Serial.print("Temperatura de ar: ");
  // Serial.println(airTemperature, 1);
  // Serial.print("Humidade de ar: ");
  // Serial.println(airHumidity, 0);
  // Serial.print("Humidade de solo: ");
  // Serial.println(100 - soilHumidityPercentage);
  // Serial.print("Porcentagem de luz: ");
  // Serial.print(analogRead(LIGHTSENSORPIN));
  // Serial.print(" - ");
  // Serial.println(lightPercentage);
  // Serial.print("Estado da bomba: ");
  // Serial.println(pumpRelay ? '1' : '0');
  // Serial.println('*');

  delay(250);
  // delay(2000);
}

// String addLeadingZeros(int n){
//   char buffer[3];
//   sprintf(buffer, "%03d", n);
//   return buffer;
// }