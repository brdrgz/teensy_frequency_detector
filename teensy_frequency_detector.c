#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include "usb_serial.h"
#include "sampling.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz	0x00
#define CPU_125kHz	0x07
#define HEX(n) (((n) < 10) ? ((n) + '0') : ((n) + 'A' - 10))

int main(void)
{
  uint16_t val;
  char buf[4];

  CPU_PRESCALE(CPU_125kHz);
  _delay_ms(1);		// allow slow power supply startup
  CPU_PRESCALE(CPU_16MHz); // set for 16 MHz clock

  // only pin F0 has an analog signal
  DIDR0 = 0x01;

  // set CO,C1 to output
  DDRC = 0x03;

  // turn off all ports
  PORTB, PORTC, PORTD, PORTE = 0x00;

  usb_init();
  while (!usb_configured()) {
    PORTC = 2;
    _delay_ms(1000);
  }
  _delay_ms(1000);

  adc_start(ADC_MUX_PIN_F0, ADC_REF_POWER);
  usb_serial_putchar('a');
  while (1) {
    val = adc_read();

    buf[0] = HEX((val >> 8) & 15);
    buf[1] = HEX((val >> 4) & 15);
    buf[2] = HEX(val & 15);
    buf[3] = ' ';

    usb_serial_write((unsigned char *)buf, 4);

    // mic is clipping
    PORTC = (val == 1023 || val == 0) ? 2 : 0;

    // adc resolution / system voltage = adc reading / measured voltage
    // 1023 / 5 = x / v_x
    // mic sits at 1.66V (silence)
    // adc == 340
    // scope analysis shows sound pickup at 1.7V (moderate volume)
    // adc >= 348
    if (val >= 348) {
      PORTC |= 1;
    } else {
      PORTC &= ~1;
    }

    // need to be able to actually see the LED!
    _delay_ms(1);
  }
}


