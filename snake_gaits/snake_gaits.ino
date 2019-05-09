/* Extreme values and pin number for each servo
OCR0A - servo1 - PD6 
  min = 
  middle = 19
  max = 
OCR0B - servo2 - PD5 
  min = 
  middle = 20
  max = 
OCR2A - servo3 - PB3 
  min = 
  middle = 19
  max = 
OCR2B - servo4 - PD3 
  min = 
  middle = 19
  max = 
*/
#define PI 3.14159265
#include <avr/io.h>
#include <util/delay.h>
double t = 0;

void setup_timer0_pwm (void)
{
  // set PD5 and PD6 as output (these are the OC0A and OC0B pins)(pin D5 and pin D6 on Nano)
  DDRD |= (1 << PD6) | (1 << PD5);
  
  // non-inverting modes for OCR0A and OCR0B and setting fast PWM
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

  // prescaling by 1024
  TCCR0B |= (1 << CS02) | (1 << CS00);
}

void setup_timer2_pwm (void)
{
  // set PD3 and PB3 as output (these are the OC2B and OC2A pins)(pin D3 and pin D11 on Nano)
  DDRB |= (1 << PB3);
  DDRD |= (1 << PD3);

  // non-inverting modes for OCR2A and OCR2B and setting fast PWM
  TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);

  // prescaling by 1024
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
}

void forward(void)
{
  for (int i = 0; i < 100; i++)
  {
    float v1 = 70 * (float(i) / 100);
    float v2 = -v1 - asin((166.26 / 183.23) * sin(v1 * PI / 180)) * (180 / PI);
    float v1_constr = map(v1, 0, 70, 19, 34);
    float v2_constr = map(v2, -128, 0, 20, 30);
    OCR0A = int(v1_constr);
    Serial.println(v2_constr);
    OCR2A = int(v2_constr);
    OCR0B = 19;
    OCR2B = 19;
    _delay_ms(5);
  }
  return;
}

double sine_forward(double t)
{
  double angle_1 = 70 * sin((5 * PI * t / 6));
  //    Serial.println(angle_1);
  OCR0A = (map(angle_1, -70, 70, 12, 29));
  double angle_3 = 70 * sin((5 * PI * t / 6) - (2 * PI / 3));
  //    Serial.println(angle_3);
  OCR2A = (map(angle_3, -70, 70, 12, 29));

  OCR0B = 19;
  OCR2B = 19;
  _delay_ms(20);
  t += 0.025;
  return t;
}

double roll(double t)
{
  double angle_1 = 60 * sin((5 * PI * t / 6) + (1 * PI / 2) + PI / 6);
  //    Serial.println(angle_1);
  OCR0A = (map(angle_1, -60, 60, 12, 29));
  double angle_3 = 60 * sin((5 * PI * t / 6) + (3 * PI / 2) + PI / 6);
  //    Serial.println(angle_3);
  OCR2A = (map(angle_3, -60, 60, 12, 29));
  
  double angle_2 = 60 * sin((5 * PI * t / 6) + (2 * PI / 2));
  //    Serial.println(angle_2);
  OCR0B = (map(angle_2, -60, 60, 13, 30));
  double angle_4 = 60 * sin((5 * PI * t / 6) + (4 * PI / 2));
  //    Serial.println(angle_4);
  OCR2B = (map(angle_4, -60, 60, 12, 29));
  _delay_ms(20);
  t += 0.025;
  return t;
}

double rotate(double t)
{
  double angle_1 = 30 * sin((5 * PI * t / 6) + PI / 2);
  //    Serial.println(angle_1);
  OCR0A = (map(angle_1, -30, 30, 12, 29));
  double angle_3 = 30 * sin((5 * PI * t / 6) + PI / 2);
  //    Serial.println(angle_3);
  OCR2A = (map(angle_3, -30, 30, 12, 29));
  
  double angle_2 = 30 * sin((5 * PI * t / 6));
  //    Serial.println(angle_2);
  OCR0B = (map(angle_2, -30, 30, 13, 30));
  double angle_4 = 30 * sin((5 * PI * t / 6));
  //    Serial.println(angle_4);
  OCR2B = (map(angle_4, -30, 30, 12, 29));
  _delay_ms(20);
  t += 0.025;
  return t;
}

double lateral_shift(double t)
{
  double angle_1 = 30 * sin((5 * PI * t / 6));
  //    Serial.println(angle_1);
  OCR0A = (map(angle_1, -30, 30, 14, 24));
  double angle_3 = 30 * sin((5 * PI * t / 6));
  //    Serial.println(angle_3);
  OCR2A = (map(angle_3, -30, 30, 14, 24));
  
  double angle_2 = 30 * sin((5 * PI * t / 6) + PI / 2);
  //    Serial.println(angle_2);
  OCR0B = (map(angle_2, -30, 30, 15, 25));
  double angle_4 = 30 * sin((5 * PI * t / 6) + 3 * PI / 2);
  //    Serial.println(angle_4);
  OCR2B = (map(angle_4, -30, 30, 14, 24));
//  OCR2B = 27;
  _delay_ms(20);
  t += 0.025;
  return t;
}

void two_rolls_and_straighten(void)
{
  straighten();
  _delay_ms(100);
  
  for (int k = 0; k < 80; k++)  
    t = roll(t);

  straighten();
  _delay_ms(100);
}

double straighten()
{
  OCR0A = 19;
  OCR2A = 19;
  OCR0B = 19;
  OCR2B = 19;
  _delay_ms(20);
}

int main (void)
{
  setup_timer0_pwm();
  setup_timer2_pwm();
  Serial.begin(9600);
//    // center position
//    OCR0A = 13;
////    OCR0B = 19;
//    OCR2A = 14;
////    OCR2B = 19;
//    _delay_ms(100);
//    OCR0A = 19;
////    OCR0B = 25;
//    OCR2A = 19;
////    OCR2B = 25;
//    _delay_ms(100);
//    OCR0A = 25;
////    OCR0B = 19;
//    OCR2A = 26;
////    OCR2B = 19;
//    _delay_ms(100);
//    OCR0A = 13;
//    OCR0B = 13;
//    OCR2A = 13;
//    OCR2B = 13;
//    _delay_ms(500);
    // rectilinear motion
//    forward();
//  for (int k = 0; k < 40; k++)
//  {
//    t = lateral_shift(t);
//  }
  while (1)
  {
    t = sine_forward(t);
  }
}
