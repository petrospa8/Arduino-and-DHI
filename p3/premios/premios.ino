#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>
#include <avr/wdt.h>

const int pulsador = 7;
int address;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {
  pinMode(pulsador, INPUT);
  analogWrite(6,90);
  Wire.begin();
  address = findI2C();
  Serial.begin(9600);
  Serial.write("start");
  lcd.begin(16, 2);
  wdt_enable(WDTO_8S);
  randomSeed(analogRead(0));
  Wire.beginTransmission(address);
  Wire.write(15);
  Wire.endTransmission();
  if(digitalRead(pulsador)){
    factoryValues();
  }
  delay(50);
  while(digitalRead(pulsador));
  showPrizes();
}

void loop() {
  wdt_reset();
  if(digitalRead(pulsador)){
    chooseWinner();
  }
}

void showPrizes(){
  byte p1 = EEPROM.read(0);
  byte p2 = EEPROM.read(1);
  byte p3 = EEPROM.read(2);
  byte p4 = EEPROM.read(3);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("p1:");
  lcd.print(p1);
  if(p1<10){
    lcd.print(" ");
  }
  lcd.print("    p2:");
  lcd.print(p2);
  lcd.setCursor(0, 1);
  lcd.print("p3:");
  lcd.print(p3);
  if(p3<10){
    lcd.print(" ");
  }
  lcd.print("    p4:");
  lcd.print(p4);
}

int ledRoulette(int finalPos){
  unsigned long t = millis();
  int i = 3;
  int retraso;
  retraso = 100;
  wdt_reset();
  while((millis() - t) < 6500){
    i = (i + 1) % 4;
    Wire.beginTransmission(address);
    Wire.write(~(1 << i));
    Wire.endTransmission();
    if(i == 3)
      retraso = retraso + 50;
    delay(retraso);
  }
  wdt_reset();
  while(i != finalPos){
    i = (i + 1) % 4;
    Wire.beginTransmission(address);
    Wire.write(~(1 << i));
    Wire.endTransmission();
    if(i == 3)
      retraso = retraso + 50;
    delay(retraso);
  }
  updateResult(finalPos);
  showPrizes();
}

void chooseWinner(){
  short p1 = EEPROM.read(0);
  short p2 = EEPROM.read(2);
  short p3 = EEPROM.read(4);
  short p4 = EEPROM.read(6);
  short p = p1 + p2 + p3 +p4;
  if(p == 0){
    return;
  }
  else{
    short N = random(1,p);
    int winner = -1;
    if(N <= p1){
      winner = 0;
    }else if(N <= p1 + p2){
      winner = 1;
    }else if(N <= p1 + p2 + p3){
      winner = 2;
    }else{
      winner = 3;
    }
    ledRoulette(winner);
  }
}
  
int findI2C(){
  for (int i= 3; i<= 0x77; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission(i)== 0) {
      return i; 
    }
  }
}

void factoryValues(){
  EEPROM.update(0, 1);
  EEPROM.update(1, 2);
  EEPROM.update(2, 5);
  EEPROM.update(3, 10);
}

void updateResult(byte result){
  EEPROM.write(result, EEPROM.read(result) - 1);
}
