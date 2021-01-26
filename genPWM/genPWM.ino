#include <MsTimer2.h>

// Valores constantes
const float pi = 3.1416;
const float Vref= 5.0;
const unsigned int rango = 1024;

const unsigned int Ts = 1; // Intervalo entre muestras en ms
const int button1 = 2;
const int button2 = 4;

// Variables globales
volatile float N = 200; // Número de muestras por ciclo
volatile unsigned int n = 0; // Número de muestra (0 a N-1)
volatile float x, x2; // Valores calculados de función seno + DC  
volatile unsigned int acc = 0;
volatile unsigned int ds = 0;
volatile unsigned int A_in;
volatile unsigned int varA, A;
void setup()  
{  
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  Serial.begin(2000000);  
  Serial.println("Mat+5:,PWM:");
  TCCR1B &= 0xF8; // Pone cero en bits preescaler PWM 9 y 10
  TCCR1B |= 0x01; // Pone PWM 9 y 10 a frec. de 32 kHz 
  MsTimer2::set(Ts, wave0);
  MsTimer2::start();
}
    
void loop() { 
  static unsigned short mode = 0;
  static unsigned long lastRead = 0;
  unsigned long t = millis();
  
  
  while(digitalRead(button1) && digitalRead(button2)){}
  if(!digitalRead(button2)){
   while(!digitalRead(button2)){}
    cli();
        
    N += 100;
    if(N == 1100){
      N = 100;
    }
    sei();
  }
  if(!digitalRead(button1)){
    while(!digitalRead(button1)){}
    cli();
    mode = (mode + 1) % 4;
  
    switch(mode){
      case 0:
        MsTimer2::stop();
        MsTimer2::set(Ts, wave0);
        MsTimer2::start();
        break;
      case 1:
        MsTimer2::stop();
        MsTimer2::set(Ts, wave1);
        MsTimer2::start();
        break;
      case 2:
        MsTimer2::stop();
        MsTimer2::set(Ts, wave2);
        MsTimer2::start();
        break;
      case 3:
        MsTimer2::stop();
        MsTimer2::set(Ts, wave3);
        MsTimer2::start();
    }
    sei();  
  }
}

void wave0() {
  A = map(analogRead(2), 0, 1023, 0, 127);
  varA = map(analogRead(2), 0, 1023, 0, 2.5);
  x= 2.5 + varA*sin(n*2*pi/N); 
  x2 = 128 + A*sin(n*2*pi/N);
  if (++n == N) { 
    n= 0;
  }  

  analogWrite(9, x2);
  if (++ds== Ts) {
      ds= 0; // Aquisición de canal cada 'Ts*T' ms
      A_in= analogRead(0); // Adquiere A0
  }
  Serial.print(x+5);
  Serial.print(" ");
  Serial.println(A_in*Vref/rango); // Envía voltaje canal A0  
}

void wave1() {
  A = map(analogRead(2), 0, 1023, 0, 127);
  varA = map(A, 0, 127, 0, 2.5);

  if(0 <= n && n < N/4){
    x = 2.5 - varA*(0 - 4*n/N);
    x2  = 128 - A*(0 - 4*n/N);
  }else if(N/4 <= n && n < 3*N/4){
    x = 2.5 + varA*(2 - 4*n/N);
    x2 = 128 + A*(2 - 4*n/N);
  } else{
    x = 2.5 - varA*(4 - 4*n/N);
    x2 = 128 - A*(4 - 4*n/N);
  }
  if (++n >= N) { 
    n= 0;
  }  
  analogWrite(9, x2);
  if (++ds== Ts) {
      ds= 0; // Aquisición de canal cada 'Ts*T' ms
      A_in= analogRead(0); // Adquiere A0
  }
  Serial.print(x+5);
  Serial.print(" ");
  Serial.println(A_in*Vref/rango); // Envía voltaje canal A0 
}

void wave2() {
  A = map(analogRead(2), 0, 1023, 127, 255);
  varA = map(analogRead(2),0, 1023, 0, 2.5);
  if(n < N/2){
    x = varA + 2.5;
    x2 = A;
  } else{
    x = 2.5;
    x2 = 0;
  }
  if (++n >= N) { 
    n= 2.5;
  }  
 
  analogWrite(9, x2);
  if (++ds== Ts) {
      ds= 0; // Aquisición de canal cada 'Ts*T' ms
      A_in= analogRead(0); // Adquiere A0
  }
  Serial.print(x + 5);  
  Serial.print(" ");
  Serial.println(A_in*Vref/rango); // Envía voltaje canal A0  
}

void wave3() {
  A = map(analogRead(2), 0, 1023, 0, 255);
  x = map(A, 0, 255, 0, 5);
  
  analogWrite(9, A);
  if (++ds== Ts) {
      ds= 0; // Aquisición de canal cada 'Ts*T' ms
      A_in= analogRead(0); // Adquiere A0
  }
  Serial.print(x+5);
  Serial.print(" ");
  Serial.println(A_in*Vref/rango); // Envía voltaje canal A0  
}
