#ifndef ADC_AVR128DA64_H_
#define ADC_AVR128DA64_H_

#define F_CPU                           (4000000UL)         /* using default clock 4MHz*/
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "UART_1_AVR128DA64.h"
#define channel_3 0x03
#define channel_4 0x04
#define START_TOKEN 0x03
#define END_TOKEN 0xFC

void ADC0_init(void)
{
	VREF.ADC0REF = 0x5 | (1 << 7);
	ADC0.CTRLC = ADC_PRESC_DIV4_gc;        // CLK_PER divided by 4
	ADC0.CTRLA = ADC_ENABLE_bm;
	PORTD.IN  = ADC_RESSEL_12BIT_gc;
	//ADC0.MUXPOS = ADC_MUXPOS_AIN3_gc;
}

void ADC0_start(void)
{
	
	ADC0.COMMAND = ADC_STCONV_bm;
}

float ADC0_read(int pin)
{
	
	ADC0.MUXPOS = pin;
	ADC0_start();
	
	
	while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
	
	return ADC0.RES;
}
int main(void)
{
	USART1_init(9600);
	// int adcVal;
	ADC0_init();
	//ADC0_read( channel_3);

	PORTC.DIRCLR = PIN7_bm;
	PORTC.PIN7CTRL |= PORT_PULLUPEN_bm;
	while (1)
	{
		
		if (!(PORTC.IN & (PIN7_bm)))
		{
			
			USART1_sendString("SYRINGE DETECTED");
			
		}
		else
		{
			USART1_sendString("SYRINGE NOT DETECTED");
		}
		float adc= ADC0_read( channel_3);
		float sum=0.00;
		for(int i=1;i<=500;i++)
		{
			sum=sum+adc;
		}
		sum=sum/500.00;
			float size=sum/82.00;
	//	float voltage = (adc*12.60)/4095.00;
		//USART1_sendFloat(adc, 2);
		//	USART1_sendFloat(voltage, 2);
		USART1_sendString(" Syringe Size");
		USART1_sendFloat(size, 2);
		_delay_ms(1000);
		
	
		
	}
}
#endif