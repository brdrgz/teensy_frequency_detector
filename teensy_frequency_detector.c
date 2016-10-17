#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "sampling.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz	0x00
#define CPU_125kHz	0x07
#define LED_OFF 0x00
#define LED_ON 0x01
#define HEX(n) (((n) < 10) ? ((n) + '0') : ((n) + 'A' - 10))

int main(void) {
	uint16_t val;
	uint16_t val_p;
  uint16_t toggle = 0x00;
	uint16_t prev;
	uint16_t alpha;
	uint16_t rc;
	uint16_t dt;
  int start = 0;

	CPU_PRESCALE(CPU_125kHz);
	_delay_ms(1);		// allow slow power supply startup
  CPU_PRESCALE(CPU_16MHz); // set for 16 MHz clock

	_delay_ms(1000);

  // only pin F0 has an analog signal
  DIDR0 = 0x01;

  // set CO to output
  DDRC = 0x01;

  // turn off all other ports
  PORTB, PORTC, PORTD, PORTE = 0x00;

	adc_start(ADC_MUX_PIN_F0, ADC_REF_INTERNAL);

	while (1) {
		//val_p = adc_read();
    val = adc_read();

    //if (start == 0) {
    //  prev = val_p;
    //}

    //dt = 1.0/(125000);
    //rc = 1.0/(2000*2*3.14);
    //alpha = dt/(rc+dt);

    //toggle = (alpha*prev) + alpha*(val-val_p);
    //prev = toggle;

    PORTC = (val > 0 && val < 1) ? LED_ON : LED_OFF;
	}
}
