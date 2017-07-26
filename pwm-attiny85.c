#include <avr/io.h>
#include <avr/interrupt.h>
 
int main (void)
{
  // set pin 5/PB0/OC0A/OC1A-bar to be output
  // Timer/Counter0 Compare Match A output
	DDRB = (1 << DDB0);

  // Table 11-5. Select Phase correct PWM with no prescaler and top =
  // 0xFF. What is the other TOP setting for?
  TCCR0A = (1 << COM0A1) | (1 << WGM00);
  TCCR0B = (1 << CS00);
  OCR0A  = 50;
  
  while (1) {
  }
 
  return 0;
}
