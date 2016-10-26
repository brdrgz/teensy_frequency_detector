#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "sampling.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz	0x00
#define CPU_125kHz	0x07

int main(void)
{
	uint16_t val;
  //double readcount = 0.0;

	CPU_PRESCALE(CPU_125kHz);
	_delay_ms(1);		// allow slow power supply startup
	CPU_PRESCALE(CPU_16MHz); // set for 16 MHz clock

  // only pin F0 has an analog signal
  DIDR0 = 0x01;

  // set CO,C1 to output
  DDRC = 0x03;

  // turn off all ports
  PORTB, PORTC, PORTD, PORTE = 0x00;

	adc_start(ADC_MUX_PIN_F0, ADC_REF_POWER);

	while (1) {
		val = adc_read();

    // baseline_adc_val = 1023 * 1.66 / 3.3;
    // baseline_adc_val = 1023 * 1.66 / 5;
    // adc resolution / system voltage = adc reading / measured voltage
    // val >= 347.82
    if (val == 1023 || val == 0) {
      PORTC = 2;
    } else {
      PORTC = 0;
    }

    if (val >= 348) {
      PORTC |= 1;
    } else {
      PORTC &= ~1;
    }

    _delay_ms(1);
    val = 0;
	}
}


