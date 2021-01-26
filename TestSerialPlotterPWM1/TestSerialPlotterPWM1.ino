          
// Genera una señal sinusoidal mediante PWM en pin 9 +
// filtro paso bajo y la adquiere en entrada analógica A0. 
// Envía al puerto serie A0 para visualización en Serial Plotter.
// José J. Lamas. UDC. 2020.

  #include <MsTimer2.h>
  
  // Valores constantes
  const float Vref= 5.0; // Voltaje de referencia del ADC (por defecto)
  const unsigned int rango= 1024; // Rango ADC de Arduino Uno (10 bits)  
  const float pi= 3.1416;
  const unsigned int T= 1; // Intervalo entre puntos en ms
  const unsigned int N= 200; // Número de puntos por ciclo
  const unsigned int Tg= 1; // Intervalo de generación en ms  
  const unsigned int Ts= 1; // Intervalo de muestreo en ms
  
  // Variables globales
  volatile unsigned int n= 0; // Número de punto (0 a N-1)
  volatile unsigned int dg= 0; // Divisor de gen. de ms(0 a Tg-1)
  volatile unsigned int ds= 0; // Divisor de adq. de ms(0 a Ts-1)
  volatile unsigned int A_in; // Valor adquirido en A0  
  volatile byte ch1; 
  
  void setup()  
  {  
    Serial.begin(2000000);  
    Serial.println("Mat+5:,PWM:");
    TCCR1B &= 0xF8; // Pone cero en bits preescaler PWM 9 y 10
    TCCR1B |= 0x01; // Pone PWM 9 y 10 a frec. de 32 kHz   
    MsTimer2::set(T, actualizar); // Int. T2 cada T ms
    MsTimer2::start();
  }  
      
  void loop() { // El bucle principal no hace nada  
  }

  // Rutina de atención de interrupción de T2 cada T ms
  void actualizar() {
    if (++dg== Tg) {
      dg= 0; // Salidas PWM para generación cada 'Tg*T' ms
      ch1= 128+127*sin(n*2*pi/N) ; // Calcula punto de CH1    
      analogWrite(9, ch1); // Salida de CH1 por pin3 PWM
    }
    if (++n== N) { // Incrementa número de punto
    n= 0;
    }  
    if (++ds== Ts) {
      ds= 0; // Aquisición de canal cada 'Ts*T' ms
      A_in= analogRead(0); // Adquiere A0
    }
    // Envía dato cada T ms a Serial Plotter
    Serial.print(5+4*ch1*Vref/rango); // Envía voltaje matemático/4
    Serial.print(" ");
    Serial.println(A_in*Vref/rango); // Envía voltaje canal A0  
  }
  
