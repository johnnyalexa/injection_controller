/*
 * injection_controller.c
 *
 * Created: 5/7/2015 8:48:21 PM
 *  Author: John
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>

#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL
#include <util/delay.h>


#define TMAX			37500UL
#define GET_T(freq)		TMAX - (117UL * freq)
#define GET_T1(period, duty)	(double)((double)period * (double)duty) / 255
#define GET_T2(period, t1)		(double)period - (double)t1

void CpuInit(void);
void ADC_Init(void);



int main(void)
{
	int i;
	unsigned int injector;
	unsigned char frequency_pc0; // Variable to hold ADC result
	unsigned char duty_cycle_pc1;
	double t,t1,t2;
	
	CpuInit();
    while(1)
    {
        //TODO:: Please write your application code 
		ADMUX &= ~(1<<MUX0); // PC0
		ADCSRA |= (1<<ADSC); // Start conversion
			while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
		
		frequency_pc0 = ADCH; //Store ADC value
		
		ADMUX |= (1<<MUX0); //PC1
		ADCSRA |= (1<<ADSC); // Start conversion
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
		
		duty_cycle_pc1 = ADCH; //Store ADC value
		
		t = GET_T(frequency_pc0);
		t1 = GET_T1(t,duty_cycle_pc1);
		t2 = GET_T2(t,t1);
		
		//for(i = 0; i<10; i++){
			for(injector=0;injector<4;injector++){
				PORTD |= 1<<injector;
				_delay_us(t1);
				PORTD &= ~(1<<injector);
				_delay_us(t2);
			}
		
		//}
		
		
    }
}

void CpuInit(void){
/*	
*	PC0 - Frequency
*	PC1 - Duty Cycle
*	PDO -> PD3 - Transistor control
*/
	//port init
	PORTD	= 0x00;
	DDRD	= 0x0F; //injection out
	PORTC	= 0x00;
	DDRC	= 0x00;  //adc in
	PORTB   = 0x00;
	DDRB	= 0xFF;
	ADC_Init();
	
}



/*-------------------- ADC_Init       -------------------------
*    Function:    ADC_Init
*    Purpose:    Initialize ADC unit.
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void ADC_Init(void){
	//Set the Band Gap voltage as the ADC input
	ADMUX = (0<<MUX0)|(1<<ADLAR); // ADC0 enabled.
	// ADC en, 128 division factor
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS0);
}

