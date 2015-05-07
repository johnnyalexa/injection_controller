/*
 * injection_controller.c
 *
 * Created: 5/7/2015 8:48:21 PM
 *  Author: John
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>

int main(void)
{
	CpuInit();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

void CpuInit(void){
	//port init
	PORTD	= 0x00;
	DDRD	= 0x0F; //injection out
	PORTC	= 0x00;
	DDRC	= 0x00;  //adc in
	
	Init_Timer1();
	ADC_Init();
	
}

/*
 * Timer 1 Interrupt routine
 */
ISR(TIMER1_COMPA_vect){
    //OCR1A = 14400;
//    seconds++;
//    state |= ONE_SECOND_INT;    
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
	ADMUX = (1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<REFS0);
	// ADC en, 128 division factor
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS0);
}

/*-------------------- Timer1_Start  -------------------------
*    Function:    Timer1_Start
*    Purpose:    Start timer 1. After Init the timer is stopped
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Timer1_Start(void){
	// clkI/O/64 (from prescaler)
	TCCR1B |= (1<<CS11) | (1<<CS10);
}

/*-------------------- Timer1_Stop   -------------------------
*    Function:    Timer1_Stop
*    Purpose:    Stop timer 1
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Timer1_Stop(void){
	//stop timer with prescaler 64
	TCCR1B &= ~((1<<CS11) | (1<<CS10));
}

/*-------------------- Init_Timer1   -------------------------
*    Function:    Init_Timer1
*    Purpose:    Initialize timer 1
*
*    Parameters:        none
*    Returns:    none
*------------------------------------------------------------*/
void Init_Timer1(void){
	
	// set up timer with prescaler = 8
	TCCR1B = (1 << WGM12);
	//set the top value for 1 sec interrupt
	OCR1A = 14400;
	// initialize counter
	//TCNT1 = 0;
	// enable compare match interrupt
	TIMSK = (1 << OCIE1A);// | (1<<TOIE1);
}