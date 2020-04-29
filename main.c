/*
	Main SPECS:
	
	LED Displaying Binded to PORTB
	Mode Switch Binded to PORTD
	Character Controlled by PORTA
	
*/

#include <avr/io.h>
#include "_main.h"
#include "lcd_n.h"
#include "Intro.h"

int main(void)
{
    PortInit();
    LCD_Init();
    
    Intro_msgs_container();
}

