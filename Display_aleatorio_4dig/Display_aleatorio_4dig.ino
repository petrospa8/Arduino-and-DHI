// Display_aleatorio 4 dígitos
// Presenta números aleatorios de 0 a 9999 en display
// de 4 dígitos de 7 segmentos de leds.
// José J. Lamas. UDC. 2019.

#include <MsTimer2.h>

// Asignación de pines a segmentos
const byte segs[7]= {3,4,5,6,7,8,9}; //a-b-c-d-e-f-g
// Asignación de pines a dígitos
const byte digs[4]= {10,11,12,13}; // 1-2-3-4
// Segmentos que encienden cada dígito del 0 al 9
const byte digitos[10]= {0b00111111, 0b00000110, 0b01011011,
            0b01001111, 0b01100110, 0b01101101, 0b01111101,
            0b00000111, 0b01111111, 0b01101111};
// Variables globales
volatile byte bytes[]= {0,0,0,0};  // Valores dígitos

// Configuraciones
void setup() {
  for (int pin= 3; pin<= 13; pin++) { // Pone modo salidas
    pinMode(pin, OUTPUT);
  }  
  MsTimer2::set(5, multiplexar); // cada 5 ms
  MsTimer2::start();
  randomSeed(analogRead(4));
}

// Bucle principal
void loop() {
  int dato= random(0, 10000);
  bytes[0]= dato / 1000; // Calcula millares
  bytes[1]= dato % 1000 / 100; // Calcula centenas
  bytes[2]= dato % 1000 % 100 / 10; // Calcula decenas
  bytes[3]= dato % 1000 % 100 % 10; // Calcula unidades
  delay(1000);
}

// Interrupción de T2 cada 5 ms para multiplexado del display
void multiplexar() {
  static int digito= 0; // Dígito encendido (D1 a D4);
  for (int i= 0; i<= 3; i++) {
    digitalWrite(digs[i], HIGH); // Apaga los 4 dígitos
  }
  segmentos(bytes[digito]); // Saca segmentos digito
  digitalWrite(digs[digito], LOW); // Enciende dígito activo
  digito= ++digito % 4;
}
void segmentos(byte valor) {
  for(int seg= 0; seg< 7; seg++) {
    digitalWrite(segs[seg], bitRead(digitos[valor], seg));
  }
}

