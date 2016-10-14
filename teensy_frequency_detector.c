#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "sampling.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz	0x00
#define CPU_125kHz	0x07
#define HEX(n) (((n) < 10) ? ((n) + '0') : ((n) + 'A' - 10))

int main(void) {
	uint16_t val;
	char buf[4];

	CPU_PRESCALE(CPU_125kHz);
	_delay_ms(1);		// allow slow power supply startup
	CPU_PRESCALE(CPU_16MHz); // set for 16 MHz clock

	_delay_ms(1000);

  // only pin F0 has an analog signal
  DIDR0 = 0x01;
  DIDR2 = 0x00;

	adc_start(ADC_MUX_PIN_F0, ADC_REF_POWER);
	while (1) {
		// read the next ADC sample, and send it as ascii hex
		val = adc_read();
		buf[0] = HEX((val >> 8) & 15);
		buf[1] = HEX((val >> 4) & 15);
		buf[2] = HEX(val & 15);
		buf[3] = ' ';
		// usb_serial_write((unsigned char *)buf, 4);
	}
}
