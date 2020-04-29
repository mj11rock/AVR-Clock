#define LCD_WDATA 	PORTA // LCD 데이터 포트 정의
#define LCD_WINST 	PORTA
#define LCD_CTRL 	PORTG // LCD 제어포트 정의
#define LCD_EN 		0
#define LCD_RW		1
#define LCD_RS 		2

#define On			1
#define Off			0
#define RIGHT		1
#define LEFT 		0

 void PortInit(void)
 {
	DDRA = 0xFF; // PORTA를 출력으로
	DDRG = 0x0F; // PORTC의 하위 4비트를 출력으로
 }
 
 void LCD_Data(char ch) 	            // LCD_DR에 데이터 출력
 {
	LCD_CTRL |= (1 << LCD_RS);       // RS=1, =0으로 데이터 쓰기 사이클
	LCD_CTRL &= ~(1 << LCD_RW);
	LCD_CTRL |= (1 << LCD_EN);       // LCD Enable
	_delay_us(50);                                // 시간지연
	LCD_WDATA = ch; // 데이터 출력
	_delay_us(50);                                // 시간지연
	LCD_CTRL &= ~(1 << LCD_EN);    // LCD Disable
 }
 
 void LCD_Comm(char ch)	            // LCD IR에 명령어 쓰기
 {
	LCD_CTRL &= ~(1 << LCD_RS);   // RS==0으로 명령어 쓰기 사이클
	LCD_CTRL &= ~(1 << LCD_RW);
	LCD_CTRL |= (1 << LCD_EN);      // LCD Enable
	_delay_us(50);                               // 시간지연
	LCD_WINST = ch;                         // 명령어 쓰기
	_delay_us(50);                               // 시간지연
	LCD_CTRL &= ~(1 << LCD_EN);   // LCD Disable
 }
 
 void LCD_CHAR(char c) 	// 한 문자 출력
{
	LCD_Data(c);
	_delay_ms(1);
}

void LCD_STR(char *str) 	// 문자열 출력
{
	while(*str != 0) {
		LCD_CHAR(*str);
		str++;
	}
}

void LCD_pos(unsigned char row, unsigned char col) // LCD 포지션 설정
{
	LCD_Comm(0x80|(row+col*0x40)); // row = 문자행, col = 문자열
}

void Cursor_Home(void)
{
	LCD_Comm(0x02); //Cursor Home
	_delay_ms(2);
}

void LCD_Clear(void)	// 화면 클리어 (1)
{
	LCD_Comm(0x01);
	 _delay_ms(2);
}

void LCD_Shift(char p)
{
	if (p == RIGHT)
	{
		LCD_Comm(0x1c);
		_delay_ms(1);
	}
	
	else if(p == LEFT)
	{
		LCD_Comm(0x18);
		_delay_ms(1);
	}
}

void Cursor_shift(char p)
{
	if(p == RIGHT)
	{
		LCD_Comm(0x14);
		_delay_ms(1);
	}
	
	else if(p == LEFT)
	{
		LCD_Comm(0x10);
		_delay_ms(1);
	}
}

void LCD_Init(void) 	// LCD 초기화
{
	LCD_Comm(0x38);	// DDRAM, 데이터 8비트사용, LCD 2열로 사용 (6)
	 _delay_ms(2); 		// 2ms 지연
	LCD_Comm(0x38); 	// DDRAM, 데이터 8비트사용, LCD 2열로 사용 (6)
	 _delay_ms(2);		// 2ms 지연
	LCD_Comm(0x38); 	// DDRAM, 데이터 8비트사용, LCD 2열로 사용 (6)
	 _delay_ms(2);		// 2ms 지연
	LCD_Comm(0x0e);	// Display ON/OFF
	 _delay_ms(2);		// 2ms 지연
	LCD_Comm(0x06);	// 주소+1 , 커서를 우측 이동 (3)
	 _delay_ms(2);		// 2ms 지연
	LCD_Clear();		// LCD 화면 클리어
}