// Saca hola y tiempo desde reset a LCD 
#include <LiquidCrystal.h>
// Inicia variable lcd y asigna pines
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
  // Configura número de columnas y filas 
  lcd.begin(16, 2);
  // Saca mensaje 
  lcd.print("Ola rapaces");
  // Genera tensión contraste en pin 6
  analogWrite(6,60);
}

void loop() {
  // Cursor a inicio segunda línea
  lcd.setCursor(0, 1);
  // Saca número de segundos desde reset:
  //lcd.print(millis()/1000);
  delay(100);
}
