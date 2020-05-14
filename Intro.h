/*
 * Intro.h
 *
 * Created: 29.04.2020 18:29:47
 *  Author: mj11r
 */ 

void Intro_msgs_container(void);
void Display_msg(char *topMsg, char *botMsg);

#ifndef INTRO_H_
#define INTRO_H_

void Intro_msgs_container(void){
	
	char welcomeTopMasg[] = "** WELCOME **";
	char welcomeBotMasg[] = "****************";
	char team[] = "*** Team ***";
	char name[] = "*** NoBug ***";
	
	Display_msg(welcomeTopMasg, welcomeBotMasg);
	_delay_ms(300);
	Display_msg(team, name);
}

void Display_msg(char *topMsg, char *botMsg)
{
	int i;
	
	LCD_pos(1,0);
	LCD_STR(topMsg);
	LCD_pos(0,1);
	LCD_STR(botMsg);
	_delay_ms(1000);
	
	for(i = 0; i < 15; i++)
	{
		LCD_Shift(LEFT);
		_delay_ms(200);
	}
	
	LCD_Clear();
}



#endif /* INTRO_H_ */