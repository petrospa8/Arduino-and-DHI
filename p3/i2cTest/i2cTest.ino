
#include <Wire.h>
int count= 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  for (int i= 3; i<= 0x77; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission(i)== 0) {
      Serial.print("I2C device ");
      Serial.print(++count);
      Serial.print(" in address= 0x");
      Serial.println(i,HEX);  
    }
  }
  if (count == 0) Serial.println("No I2C devices found");
  Serial.println("");
  delay(2000);
  count= 0;
}
