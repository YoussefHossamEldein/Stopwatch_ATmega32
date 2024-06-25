/* ==========================================================
 * Name        : mini_project_2.c
 * Description : Stop Watch using TIMER1 Module
 * Date        : Sep 16, 2023
 * Author      : Youssef Hossam Eldein Mohamed
 * ==========================================================
 */
/***************** Including Section ******************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"
/********************** Global Declaration Section********************/
/*	  second1 : Global Variable for the counting of second units
 * 	  second2 : Global Variable for the counting of second tens
 * 	  minute1 : Global Variable for the counting of minute units
 * 	  minute2 : Global Variable for the counting of second tens
 * 	  hour1   : Global Variable for the counting of hour units
 * 	  hour2   : Global Variable for the counting of hour tens
 */
unsigned char second1,second2,minute1,minute2,hour1,hour2;
/************ Interrupt service routines section ****************/
// Interrupt service routine for TIMER1 compare mode
ISR(TIMER1_COMPA_vect){
	second1++;
	if(second1 == 9){
		second1 = 0;
		second2++;
	}
	if (second2 == 6){
		second2 = 0;
		minute1++;
	}
	 if(minute1 == 9){
		minute1 = 0;
		minute2++;
	}
	 if(minute2 == 6){
		 minute2 = 0;
		 hour1++;
	 }
	 if(hour1 == 4){
	 hour1 = 0;
	hour2++;
	 }

	 if(hour2 == 2){
		hour2 = 0;
	 }
}
//Interrupt service routine for external interrupt 0
ISR(INT0_vect){
	second1 = 0;
	second2 = 0;
	minute1 = 0;
	minute2 = 0;
	hour1 = 0;
	hour2 = 0;
}
//Interrupt service routine for external interrupt 1
ISR(INT1_vect){
	TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10); //No Clock Source(Timer/counter stopped
}
//Interrupt service routine for external interrupt 2
ISR(INT2_vect){
	TCCR1B |= (1<<CS12) | (1<<CS10); //
}
/**************** Functions Definition Section *************************/
/* =====================================================================
 * @brief  : Function to configure TIMER1 on compare mode
 * @param  : function takes nothing
 * @retval : function returns nothing
 * =====================================================================
 */
void TIMER1_INIT(void){
	TCNT1   = 0; //Set Timer Initial Value 0
	OCR1A  = 1000; //Set Compare Value 1000
	TIMSK  |= (1<<OCIE1A); // Enable TIMER1 Interrupt on compare match
	/* Configure timer control register
		 * 1. Non PWM mode FOC1A = 1
		 * 2. CTC Mode WGM12=1
		 * 3. clock = F_CPU/1024 CS12 = 1 CS10 = 1 CS11 = 0
		 *
		 */
	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<CS12) | (1<<CS10) | (1<<WGM12);


}
/* =====================================================================
 * @brief  : Function to activate external interrupt 0 with falling edge
 * @param  : function takes nothing
 * @retval : function returns nothing
 * =====================================================================
 */
void RESET(void){
	DDRD  &= ~(1<<PD2);//configure pin2 in PORTD as an input pin
	PORTD |= (1<<PD2); //Activate the internal pull up
	MCUCR |= (1<<ISC01);//trigger the interrupt with falling edge
	GICR  |= (1<<INT0);//Enable the module interrupt
}
/* =====================================================================
 * @brief  : Function to activate external interrupt 1 with rising edge
 * @param  : function takes nothing
 * @retval : function returns nothing
 * =====================================================================
 */
void PAUSE(void){
	DDRD  &= ~(1<<PD3);//configure pin3 in PORTD as an input pin
	MCUCR |= (1<<ISC11) | (1<<ISC10);//trigger the interrupt with rising edge
	GICR  |= (1<<INT1); //Enable the module interrupt
}
/* =====================================================================
 * @brief  : Function to activate external interrupt 2 with falling edge
 * @param  : function takes nothing
 * @retval : function returns nothing
 * =====================================================================
 */
void PLAY(void){
	DDRB   &= ~(1<<PB2);//configure pin2 in PORTB as an input pin
	PORTB |= (1<<PB2); //Activate the internal pull up
	MCUCSR &= ~(1<<ISC2); //trigger the interrupt with falling edge
	GICR   |= (1<<INT2);//Enable the module interrupt
}

void main(void){
	DDRA = 0x3F; /* configure first 6 pins of PORTA (control pins of 7segments) as output pins */

	PORTA = 0xFF;

	DDRC = 0x0F; /* configure first 4 pins in PORTC (connected with 7447 decoder) as output pins*/

	PORTC = 0x00; /* Display 0 at the beginning */

	SREG   |= (1<<7);  // Enable global interrupts in MC

    TIMER1_INIT();

    RESET();

    PAUSE();

    PLAY();
while(1){


			PORTA = 0x01;
			PORTC = second1;
			_delay_ms(4);
			PORTA = 0x02;
			PORTC = second2;
			_delay_ms(4);
			PORTA = 0x04;
			PORTC = minute1;
			_delay_ms(4);
			PORTA = 0x08;
			PORTC = minute2;
			_delay_ms(4);
			PORTA = 0x10;
			PORTC = hour1;
			_delay_ms(4);
			PORTA = 0x20;
			PORTC = hour2;
			_delay_ms(4);

}
}
