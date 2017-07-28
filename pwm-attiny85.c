#include <avr/io.h>
#include <avr/interrupt.h>
 
int main (void)
{
  // set pin 5/PB0/OC0A/OC1A-bar to be output
  // Timer/Counter0 Compare Match A output
	DDRB = (1 << DDB0);

  // Table 11-5. Select Phase correct PWM with no prescaler and top =
  // 0xFF. What is the other TOP setting for? Apparently in Mode 5,
  // OCR0A is the TOP value and you can only get a PWM on OC0B. Why
  // would you want that? Maybe to control the frequency as well as
  // duty cycle.
  TCCR0A = (1 << COM0A1) | (1 << WGM00);
  TCCR0B = (1 << CS01);
  OCR0A  = 150;
  
  while (1) {
  }
 
  return 0;
}
