#include <MsTimer2.h>

// Asignación de pines a segmentos
const byte segs[7]= {3,4,5,6,7,8,9}; //a-b-c-d-e-f-g
// Asignación de pines a dígitos
const byte digs[4]= {10,11,12,13}; // 1-2-3-4
// Segmentos que encienden cada dígito del 0 al 9
const byte digitos[14]= {0b00111111, 0b00000110, 0b01011011,
            0b01001111, 0b01100110, 0b01101101, 0b01111101,
            0b00000111, 0b01111111, 0b01101111, 0b00111001/*c*/,
            0b01100011/*o*/, 0b00000000 /*blank*/, 0b01000000/*-*/};

const float A = 1.11492089e-3;
const float B = 2.372075385e-4;
const float C = 6.954079529e-8;

// Variables globales
volatile float acuTemp = 0;
volatile byte bytes[]= {0,0,0,0};  // Valores dígitos
volatile float Vo;
volatile short i;
volatile int k = 0; //indice para iterar no array das ultimas 5 medias
volatile int mean = 0;
volatile float Rm, T, celsius, Vm;
volatile float lastMeans[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
volatile int iteration = 0;

// Configuraciones
void setup() {
  Serial.begin(230400);
  for (int pin= 3; pin<= 13; pin++) { // Pone modo salidas
    pinMode(pin, OUTPUT);
  }  
  MsTimer2::set(2, medir);
  MsTimer2::start();
}


void loop() {}

// Interrupción de T2 cada 5 ms para multiplexado del display
void multiplexar() {
  static int digito= 0; // Dígito encendido (D1 a D4);
  for (int i= 0; i<= 3; i++) {
    digitalWrite(digs[i], HIGH); // Apaga los 4 dígitos
  }
  segmentos(bytes[digito]); // Saca segmentos digito
  digitalWrite(digs[digito], LOW); // Enciende dígito activo
  digito = (digito + 1) % 4;
}
void segmentos(byte valor) {
  for(int seg= 0; seg< 7; seg++) {
    digitalWrite(segs[seg], bitRead(digitos[valor], seg));
  }
}

void medir(){
  multiplexar();
  acuTemp += (float) analogRead(0);
  iteration = (iteration + 1) % 100;
  if(iteration == 0){
    mean = acuTemp/100.0;
    acuTemp = 0.0; 
    Rm = (float) ((10000. * mean) /(1024. - mean));
    T = (float) (1 / (A+B*log(Rm)+C*log(Rm)*log(Rm)*log(Rm)));
    Vm = (float) ((5 * mean) / 1024);
    celsius = (float) (T - ((1000* Vm * Vm) / (2.5*Rm)) - 273.15);
    lastMeans[k] = celsius;
    if(++k > 4){
      k = 0;
    }
    celsius = 0.0;
    for(i = 0; i <= 4; i++){
      celsius = (float)(celsius + lastMeans[i]);
    }
    celsius = (float) (celsius / 5.0);
    if(celsius > 9){
      bytes[0]= (int) celsius / 10; // Calcula decenas
      bytes[1]= (int) celsius % 10; // Calcula unidades
      bytes[2] = 11;
      bytes[3] = 10;
    }else if(celsius >= 0 && celsius < 10 ){
      bytes[0] = 12;
      bytes[1] = (int) celsius % 10; // Calcula unidades
      bytes[2] = 11;
      bytes[3] = 10;
    } else if(celsius > -1 && celsius < 0){
      bytes[0] = 13;
      bytes[1] = abs((int) celsius) % 10; // Calcula unidades
      bytes[2] = 11;
      bytes[3] = 10;
    } else{
      bytes[0] = 13;
      bytes[1]= abs((int) celsius) / 10; // Calcula decenas
      bytes[2]= abs((int) celsius) % 10; // Calcula unidades
      bytes[3] = 11;
    }
    Serial.print("Time(ms): ");
    Serial.print(millis());
    Serial.print(" Rm(ohm.): ");
    Serial.print(Rm);
    Serial.print(" T(ºC): ");
    Serial.println(celsius);
 
  }
}
