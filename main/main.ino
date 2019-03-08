#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  // set PD5 and PD6 as output (these are the OC0A and OC0B pins)(pin D5 and pin D6 on Nano)
  DDRD |= (1 << PD6) | (1 << PD5);
  
  // non-inverting modes for OCR0A and OCR0B and setting fast PWM
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

  // prescaling by 1024
  TCCR0B |= (1 << CS02) | (1 << CS00);

  while (1)
  {
    OCR0B = 13;
    OCR0A = 25;
    _delay_ms(500);
    OCR0B = 25;
    OCR0A = 13;
    _delay_ms(500);
  }
}
