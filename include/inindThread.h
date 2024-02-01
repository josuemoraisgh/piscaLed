// O timer0 é utilizado pelo Arduino para funções como delay(), millis() e micros(). Então não se deve utilizar esse timer para evitar comprometer essas funções.
// O timer1 no Arduino UNO esse é o timer utilizado pela biblioteca de controle de servos. Caso você não esteja utilizando essa biblioteca, esse timer está livre para ser utilizado para outros propósitos. No Arduino Mega esse timer só será utilizado para controlar os servos se você estiver usando mais de 12 servos. (timer utilizado no projeto)
// O timer2 é utilizado pela função tone(). Então se você não precisar da função tone() esse timer está livre para outras aplicações.
#include <Arduino.h>
#include <stdarg.h>

#define TAMTHREAD 5
using CallbackFunc = void (*)(); // definição de tipo para nossa callback
struct threadStruct
{
  volatile CallbackFunc func[TAMTHREAD];
  volatile int interval[TAMTHREAD];
  volatile int time[TAMTHREAD];
};
threadStruct thread;
volatile int numThread;

void threadSetup(CallbackFunc callback, int threadInterval, ...)
{
  numThread = 1;
  va_list args;
  va_start(args, threadInterval);

  while (va_arg(args, CallbackFunc))
  {
    va_arg(args, int);
    numThread++;
  }

  // Dynamically allocate memory using malloc()
  // threadTime     = (int *) malloc(numThread * sizeof(int));
  // threadInterval = (int *) malloc(numThread * sizeof(int));
  // threadFunc     = (CallbackFunc *) malloc(numThread * sizeof(CallbackFunc));

  thread.time[0] = 0;
  thread.func[0] = callback;
  thread.interval[0] = threadInterval;

  va_start(args, threadInterval);
  for (int i = 1; i < numThread && i < TAMTHREAD; i++)
  {
    thread.time[i] = 0;
    thread.func[i] = va_arg(args, CallbackFunc);
    thread.interval[i] = va_arg(args, int);
  }
  va_end(args);
  cli();               // desliga as interrupções para não haver error durante a configuração
  TCCR1A = 0b01000000; // modo comparação
  TCCR1B = 0b00001011; // configuração do prescaler em 64 e OCR1A register is used to manipulate the counter resolution
  TIMSK1 = 0b00000001; // habilita a interrupção por overflow do timer
  OCR1A = 249;         // compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1
  sei();               // habilita interrupções globais
}

ISR(TIMER1_OVF_vect)
{
  for (int i = 0; i < numThread && i < TAMTHREAD; i++)
  {
    if (thread.time[i] >= thread.interval[i])
    {
      thread.func[i]();
      thread.time[i] = 0;
    }
    else
      thread.time[i]++;
  }
}
