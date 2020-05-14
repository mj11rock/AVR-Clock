#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "_main.h"
#include "lcd_n.h"
#include "Intro.h"

char LED;
int cnt;
char stop;

char alarmIntro[]="Alarm-1";
char timeIntro[]="Time-0";
char timerIntro[]="Timer-2";
char alarm[]="Alarm";
char time[]="Time";
char timer[]="Timer";
int year=1980;
char day[]="Mon";

short int sec=20, min=1, hour=1, date=28, month=10, millis;

char hour_alarm = 0;
char min_alarm = 0;
char sec_alarm = 0;

char alarm_flag = 0;
char stop;

void Led_OnOff(void);			// blinking display and show "Alert" message
void Init_Timer(void);			// initialize global timer


// *** Functions declaration *** 

void change_alarm_hour(void);		// handle alarm hour change
void change_alarm_minute(void);		// handle alarm minute change
void Interrupt_init();				// interrupt initialization	
void change_min(void);				// handle min change
void change_hour(void);				// handle hour change
void change_month(void);			// handle mount change
void change_date(void);				// handle date change
void showTime(void);				// Place Time and date in asked format
void Normal_Time(void);				// Clock logic function
void Stopwatch(void);				// Stopwatch Logic Function
void Alarm_Time(void);				// Alarm Logic Function


// *** Interrupts configs ***

// Clock Mode
SIGNAL(INT0_vect){
	Normal_Time();
	sei();
}

// Alarm Mode
SIGNAL(INT1_vect){
	alarm_flag = 1;
	Alarm_Time();
}

// Stopwatch Mode
SIGNAL(INT2_vect){ Stopwatch(); }

// start/pause stopwatch
SIGNAL(INT3_vect){ stop = ~stop; }

// Main Time Counter
ISR(TIMER0_COMP_vect)
{
	cnt++;
	
	
	if(cnt == 144)
	{
		// check alarm state
		if(alarm_flag){
			if( (hour_alarm == hour) && (min_alarm == min) && (sec_alarm == sec) )
			{
				Led_OnOff();
				alarm_flag = 0;
			}
		}
		
		cnt=0;
		sec++;
		
		if(sec > 60 - 1){
			min++;
			sec=0;
		}
		if(min > 60 - 1){
			hour++;
			min=0;
		}
		
		if(hour > 24 - 1){
			date++;
			hour=0;
		}
		if(date > 30)
		{
			month++;
			date=0;
			
		}
		if(month > 12){
			year++;
			month=0;
		}
		
	}
}

// main flow
int main(void)
{
	Interrupt_init();
	PortInit();
	LCD_Init();
	LCD_Clear();
	
	DDRD=0x00;
	DDRB=0xff;
	Init_Timer();
	SREG |= 0x80;
	
	// LCD initialize
	PortInit();
	LCD_Init();
	Intro_msgs_container();

	LCD_Clear();
	LCD_pos(0,0);
	LCD_STR(timeIntro);
	LCD_pos(0,1);
	LCD_STR(alarmIntro);
	LCD_pos(7,0);
	LCD_STR(timerIntro);
	
	while (1)
	{
		Interrupt_init();
		
	}
}

// LED Blinking during alarm
void Led_OnOff(void){
	
	PORTB=0x00;				// disable LEDs
	LCD_Clear();			// disable LCD
	_delay_ms(500);			// delay 500ms
	PORTB = 0xff;			// enable all LEDs
	_delay_ms(500);			// delay 500ms
	PORTB=0x00;				// disable all LEDs
	LCD_pos(0,0);			// Setting message position
	LCD_STR(alarm);			// Show alarm message
	_delay_ms(1000);		// Repeat...
	PORTB = 0xff;
	LCD_Clear();
	_delay_ms(500);
	PORTB=0x00;
	LCD_pos(0,0);
	LCD_STR(alarm);
	_delay_ms(1000);
	LCD_Clear();
	
}


void Init_Timer(void)
{
	TCCR0 = 0x0f;				//	CTC mode, Prescale 1024
	OCR0 = 99;					//  count 0 - 99
	TIMSK = 0x02;				//	Output compare interrupt enable
}


void change_alarm_hour(void){
	hour_alarm++;				// increment hour
	if(hour_alarm>=24){			// check range
		hour_alarm=0;
	}
}

void change_alarm_minute(void){
	min_alarm++;				// increment minutes
	if(min_alarm>=60){			// check range
		min_alarm=0;
	}
}

void Interrupt_init(){
	EICRA=0x11;					// Int0 falling edge
	EIMSK=0xff;					// Int0 enable
	DDRD=0x00;					// PortD as input
	sei();

}
void change_min(){
	min++;						// +1 min
}

void change_hour(){
	hour++;						// +1 hour
}

void change_month(void){
	month++;					// +1 month
}

void change_date(void){
	date++;						// +1 day
}

/* 
	This function put current time information on two lines:
		*first line:  yyyy (year) dd (day) / mm (mounth) www (week day) 
		*second line: AM/PM: hh (hours):mm (minutes):ss (seconds)
*/
void showTime(void){
	char AM[ ] = "AM:";
	char PM[ ] = "PM:";
	
	LCD_pos(5,0);
	LCD_CHAR((date/10)+'0');
	LCD_CHAR((date%10)+'0');
	LCD_pos(7,0);
	LCD_CHAR('/');
	LCD_pos(8,0);
	LCD_CHAR((month/10)+'0');
	LCD_CHAR((month%10)+'0');
	LCD_pos(11,0);
	LCD_STR(day);
	
	if(hour > 12)
	{
		
		LCD_pos(0,1);
		LCD_STR(PM);
		LCD_pos(3,1);
		LCD_CHAR(((hour-12)/10)+'0');
		LCD_CHAR(((hour-12)%10)+'0');
		LCD_CHAR(':');
	}
	else
	{
		
		LCD_pos(0,1);
		LCD_STR(AM);
		LCD_pos(3,1);
		LCD_CHAR((hour/10)+'0');
		LCD_CHAR((hour%10)+'0');
		LCD_CHAR(':');
	}
	
	LCD_pos(6,1);
	LCD_CHAR((min/10)+'0');
	LCD_CHAR((min%10)+'0');
	LCD_CHAR(':');
	LCD_pos(9,1);
	LCD_CHAR((sec/10)+'0');
	LCD_CHAR((sec%10)+'0');
}

/*
	This function adds date/time configuration with intterupts 
	INT4 - +1 day
	INT5 - +1 month
	INT6 - +1 hour
	INT7 - +1 minute
*/
void Normal_Time(void){
	Interrupt_init();
	Init_Timer();
	SREG |=0x80;
	sei();
	PortInit();
	LCD_Init();
	LCD_Clear();
	
	while (1)
	{
		char buffer[4];
		LCD_pos(0,0);
		sprintf(buffer, "%d", year);
		LCD_STR(buffer);
		char key;
		key=(PIND & 0xf0);
		switch(key){				// check which btn pressed
			case 0xe0:
				_delay_ms(150);		// avoid btn sticking
				change_date();
				break;
			case 0xd0:
				_delay_ms(150);
				change_month();
				break;
			case 0xb0:
				_delay_ms(150);
				change_hour();
				break;
			case 0x70:
				_delay_ms(150);
				change_min();
				break;
			default:
				break;
		}
		showTime();
	}
}


/*
	Function contains stopwatch logic
	INT2 resets stopwatch
	INT3 starts/pauses stopwatch

*/
void Stopwatch(void){
	
	char str[]="Stopwatch";
	char seconds= 0,
	millisec= 0,
	mins=0,
	hours=0;
	int count = 0;
	Init_Timer();
	SREG |=0x80;
	PortInit();
	LCD_Init();
	LCD_Clear();
	LCD_pos(2,0);
	LCD_STR(str);
	
	while(1){
		count++;
		
		if(count == 200)
		{
			count = 0;
			millisec+=2.5;
			if(millisec == 100){
				millisec = 0;
				seconds++;
			}
			
			if(seconds >= 60) {
				mins++; seconds = 0;
			}
			if(mins >= 60) {
				hours++; mins = 0;
			}
			if(hours >= 24) hours = 0;
			
			LCD_pos(0,1);
			LCD_CHAR(hours/10+'0');
			LCD_CHAR(hours%10+'0');
			
			LCD_pos(3,1);
			LCD_CHAR((mins/10)+'0');
			LCD_CHAR((mins%10)+'0');
			LCD_CHAR(':');
			
			LCD_pos(6,1);
			LCD_CHAR((seconds/10)+'0');
			LCD_CHAR((seconds%10)+'0');
			LCD_CHAR(':');
			
			LCD_pos(9,1);
			LCD_CHAR((millisec/10)+'0');
			LCD_CHAR((millisec%10)+'0');
		}
		
		if(stop==0){
			
			LCD_pos(0,1);
			LCD_CHAR(hours/10+'0');
			LCD_CHAR(hours%10+'0');
			
			LCD_pos(3,1);
			LCD_CHAR((mins/10)+'0');
			LCD_CHAR((mins%10)+'0');
			LCD_CHAR(':');
			
			LCD_pos(6,1);
			LCD_CHAR((seconds/10)+'0');
			LCD_CHAR((seconds%10)+'0');
			LCD_CHAR(':');
			
			LCD_pos(9,1);
			LCD_CHAR((millisec/10)+'0');
			LCD_CHAR((millisec%10)+'0');
		}
	}
	
}

/*
	Function contains Alarms logic
	INT1 alarms menu

	With INT4 and INT5 Alarm time should be set 
	INT4 +1 hour
	INT5 +1 minute

*/
void Alarm_Time(void){
	DDRB = 0xff;
	Interrupt_init();
	Init_Timer();
	SREG |= 0x80;
	char set[] = "Time:";
	PortInit();
	LCD_Init();
	LCD_Clear();
	int getSec = sec;
	int getMin = min;
	int getHour = hour;
	int ms = 0;
	int count = 0;
	//min_alarm = 0;
	//hour_alarm = 0;
	//sec_alarm = 0;
	
	LCD_pos(0,0);
	LCD_STR(set);
	LCD_pos(0,1);
	LCD_STR(alarm);
	
	while(1){
		
		
		count ++;
			
		char key;
		key=(PIND & 0xf0);
		
		if(count == 200)
		{
			count = 0;
			ms++;
			
			if(ms == 25){
				getSec++;
				ms = 0;
			}
			if(getSec >= 60) {
				getMin++;
				getSec = 0;
			}
			if(getMin >= 60) {
				getHour++;
				getMin = 0;
			}
			if(getHour >= 24)
				getHour = 0;
			
			switch(key){
				case 0xe0:
					_delay_ms(200);
					change_alarm_hour();
					break;
				case 0xd0:
					_delay_ms(200);
					change_alarm_minute();
					break;
				default:
					break;
			}
			
			LCD_pos(6,0);
			LCD_CHAR(getHour/10+'0');
			LCD_CHAR(getHour%10+'0');
			LCD_CHAR(':');
			LCD_pos(9,0);
			LCD_CHAR((getMin/10)+'0');
			LCD_CHAR((getMin%10)+'0');
			LCD_CHAR(':');
			LCD_pos(12,0);
			LCD_CHAR((getSec/10)+'0');
			LCD_CHAR((getSec%10)+'0');
			
			
			LCD_pos(6,1);
			LCD_CHAR(hour_alarm/10+'0');
			LCD_CHAR(hour_alarm%10+'0');
			LCD_CHAR(':');
			LCD_pos(9,1);
			LCD_CHAR(min_alarm/10+'0');
			LCD_CHAR(min_alarm%10+'0');
			LCD_CHAR(':');
			LCD_pos(12,1);
			LCD_CHAR(sec_alarm/10+'0');
			LCD_CHAR(sec_alarm%10+'0');

		}
		
	}
}
