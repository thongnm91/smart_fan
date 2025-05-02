#define F_CPU 10000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define PWM_TOP     255
#define PWM_MIN     0
#define PWM_STEP    51
#define DEBOUNCE_TIME_MS 20

volatile uint8_t duty = PWM_TOP;
volatile uint8_t debounce = 0; // 0 = ready, 1 = waiting for debounce

void pwm_init(void){
	TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); // Fast PWM, Clear OC0A on Compare Match
	TCCR0B |= (1 << CS02); // Prescaler 256
	DDRB |= (1 << PB2) | (1<<PB7) | (1<<PB6); // OC0A as output
	PORTB |= ~(1<<PB7)|(1<<PB6);
	OCR0A = duty;
}

void external_interrupts_init(void){
	GIMSK |= (1 << INT0) | (1 << INT1); // Enable INT0 and INT1
	MCUCR |= (1 << ISC01) | (1 << ISC11); // Falling edge
	PORTD |= (1 << PD2) | (1 << PD3); // Pull-up resistors
}

void debounce_timer_init(void){
	TCCR1B |= (1 << WGM12); // CTC mode
	OCR1A = (F_CPU / 1024 / 1000) * DEBOUNCE_TIME_MS; // Timer compare for ~30 ms
	TIMSK |= (1 << OCIE1A); // Enable Timer1 Compare A Match interrupt
}

ISR(INT0_vect){ // PD2 - Increase
	if (!debounce) {
		if((PIND&(1<<PD2))==0){
			if (duty + PWM_STEP < PWM_TOP)
			{
				duty += PWM_STEP;
				PORTB |= (1<<PB7)|(1<<PB6);
			}
			else
			{
				duty = PWM_TOP;
				PORTB &= ~(1<<PB7); //Indicate max
			}
			OCR0A = duty;
			debounce = 1;
			TCNT1 = 0; // Reset debounce timer
			TCCR1B |= (1 << CS12) | (1 << CS10); // Start Timer1 with 1024 prescaler
			while((PIND&(1<<PD2))==0){}
		}
	}
}

ISR(INT1_vect){ // PD3 - Decrease
	if (!debounce) {
		if((PIND&(1<<PD3))==0){
			if (duty > PWM_STEP)
			{
				PORTB |= (1<<PB7)|(1<<PB6);
				duty -= PWM_STEP;
			}
			else
			{
				duty = PWM_MIN;
				PORTB &= ~(1<<PB6);
			}
			OCR0A = duty;
			debounce = 1;
			TCNT1 = 0; // Reset debounce timer
			TCCR1B |= (1 << CS12) | (1 << CS10); // Start Timer1 with 1024 prescaler
			while((PIND&(1<<PD3))==0){}
		}
	}
}

ISR(TIMER1_COMPA_vect){
	debounce = 0; // Clear debounce lock
	TCCR1B &= ~((1 << CS12)|(1 << CS10)) ; // Stop Timer1
	//PORTB |= (1<<PB6) | (1<<PB7);
}

int main(void){
	pwm_init();
	external_interrupts_init();
	debounce_timer_init();
	sei(); // Enable global interrupts

	while (1) {
		// Everything handled in interrupts
	}
}
