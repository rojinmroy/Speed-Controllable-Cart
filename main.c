/*******************************************************
This program was created by the
CodeWizardAVR V3.21 Evaluation
Automatic Program Generator
© Copyright 1998-2015 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Speed Controllable Cart
Version : 
Date    : 6/4/2016
Author  : Rojin M. Roy
Company : 
Comments: 


Chip type               : ATmega8
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256

PC0 (ADC0)              : Potentiometer
PC2                     : Forward button
PC3                     : Left button
PC4                     : Right button
PC5                     : Reverse button
PD2                     : L293D IN2
PD3                     : L293D IN1
PD4                     : L293D EN1
PD5                     : L293D EN2
PB6                     : L293D IN3
PB7                     : L293D IN4
PB1                     : LED
PB2                     : LED
PB3                     : LED
*******************************************************/

#include <io.h>

#include <delay.h>

// Declare your global variables here

// Timer2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
	// Place your code here
	PORTD.4=1;
	PORTD.5=1;
}

// Timer2 output compare interrupt service routine
interrupt [TIM2_COMP] void timer2_comp_isr(void)
{
	// Place your code here
	PORTD.4=0;
	PORTD.5=0;
}

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (1<<ADLAR))

// Read the 8 most significant bits
// of the AD conversion result
unsigned char read_adc(unsigned char adc_input)
{
	ADMUX=adc_input | ADC_VREF_TYPE;
	// Delay needed for the stabilization of the ADC input voltage
	delay_us(10);
	// Start the AD conversion
	ADCSRA|=(1<<ADSC);
	// Wait for the AD conversion to complete
	while ((ADCSRA & (1<<ADIF))==0);
	ADCSRA|=(1<<ADIF);
	return ADCH;
}

void forward()
{
	PORTD.4=1;
	PORTD.3=0;
	PORTD.2=1;

	PORTD.5=1;
	PORTB.7=1;
	PORTB.6=0;
}

void reverse()
{
	//PORTD.4=1;
	PORTD.3=1;
	PORTD.2=0;

	//PORTD.5=1;
	PORTB.7=0;
	PORTB.6=1;
}

void right()
{
	//PORTD.4=0;
	PORTD.3=0;
	PORTD.2=1;

	//PORTD.5=1;
	PORTB.7=0;
	PORTB.6=1;
}

void left()
{
	//PORTD.4=1;
	PORTD.3=1;
	PORTD.2=0;

	//PORTD.5=1;
	PORTB.7=1;
	PORTB.6=0;
}

void stop()
{
	PORTD.4=0;
	PORTD.3=1;
	PORTD.2=0;

	PORTD.5=0;
	PORTB.7=1;
	PORTB.6=0;
}

void main(void)
{
	// Declare your local variables here

	// Input/Output Ports initialization
	// Port B initialization
	// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
	DDRB=(1<<DDB7) | (1<<DDB6) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
	// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
	PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

	// Port C initialization
	// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
	DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
	// State: Bit6=P Bit5=P Bit4=P Bit3=P Bit2=P Bit1=P Bit0=P 
	PORTC=(1<<PORTC6) | (1<<PORTC5) | (1<<PORTC4) | (1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0);

	// Port D initialization
	// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
	DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);
	// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
	PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: Timer 0 Stopped
	TCCR0=(0<<CS02) | (0<<CS01) | (0<<CS00);
	TCNT0=0x00;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: Timer1 Stopped
	// Mode: Normal top=0xFFFF
	// OC1A output: Disconnected
	// OC1B output: Disconnected
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
	TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 1000.000 kHz
	// Mode: Normal top=0xFF
	// OC2 output: Disconnected
	// Timer Period: 0.256 ms
	ASSR=0<<AS2;
	TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (1<<CS21) | (0<<CS20);
	TCNT2=0x00;
	OCR2=0x00;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK=(1<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<TOIE0);

	// External Interrupt(s) initialization
	// INT0: Off
	// INT1: Off
	MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);

	// USART initialization
	// USART disabled
	UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

	// Analog Comparator initialization
	// Analog Comparator: Off
	// The Analog Comparator's positive input is
	// connected to the AIN0 pin
	// The Analog Comparator's negative input is
	// connected to the AIN1 pin
	ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

	// ADC initialization
	// ADC Clock frequency: 1000.000 kHz
	// ADC Voltage Reference: AREF pin
	// Only the 8 most significant bits of
	// the AD conversion result are used
	ADMUX=ADC_VREF_TYPE;
	ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	SFIOR=(0<<ACME);

	// SPI initialization
	// SPI disabled
	SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

	// TWI initialization
	// TWI disabled
	TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

	// Globally enable interrupts
	#asm("sei")

	while (1) {
		// Place your code here
		if(PINC.2==0) {
			//   OCR2=read_adc(0);
			forward();

			PORTB.1=1;
			PORTB.2=0;
			PORTB.3=1;
		}
		else if(PINC.3==0) {
			OCR2=read_adc(0);
			left();
			PORTB.1=1;
			PORTB.2=1;
			PORTB.3=1;
		}
		else if(PINC.4==0) {
			OCR2=read_adc(0);
			right();
			PORTB.1=1;
			PORTB.2=0;
			PORTB.3=0;
		}
		else if(PINC.5==0) {
			OCR2=read_adc(0);
			reverse();
			PORTB.1=0;
			PORTB.2=0;
			PORTB.3=1;
		}
		else {
			stop();
		}
	}
}
