/*
 * _main.h
 *
 * Created: 2015-12-16 오후 7:00:18
 *  Author: SongYi Jung
 */ 

#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define nop()  __asm__ __volatile__ ("nop" ::)
#define NOP()  __asm__ __volatile__ ("nop" ::)
#define _NOP()  __asm__ __volatile__ ("nop" ::)

#define CLI() cli()
#define SEI() sei()
#define BIT(x)	(1 << (x))

typedef unsigned char		byte;
typedef unsigned char		u8;
typedef signed int		s16;
typedef unsigned int		u16;
typedef signed long		s32;
typedef unsigned long		u32;
typedef signed long long	s64;
typedef unsigned long long 	u64;

#define sbi(port, bit) 	(port |= (1<<bit))
#define cbi(port, bit) 	(port &= (~(1<<bit)))
#define inp(port, bit) 		(port & (1<<bit))
// bit 매크로
#define SetBit(x,y)  (x|=(1<<y))
#define ClrBit(x,y)  (x&=~(1<<y))
#define ToggleBit(x,y) (x^=(1<<y))
#define FlipBit(x,y) (x^=(1<<y)) // Same as ToggleBit.
#define TestBit(x,y) (x&(1<<y))

// bit 매크로
#define SETBIT(x,y)  (x|=(1<<y))
#define CLEARBIT(x,y)  (x&=~(1<<y))
#define TOGGLEBIT(x,y) (x^=(1<<y))
#define FLIPBIT(x,y) (x^=(1<<y)) // Same as ToggleBit.
#define TESTBIT(x,y) (x&(1<<y))

extern unsigned int INT0_DataView;
extern unsigned int INT1_DataView;
extern unsigned int Uart1_DataView;


extern int game_screen; //게임 화면 선택 (0:start, 1:info, 2:action, 3:over, 4:score)
extern int game_score; //게임 점수


void game_init_action(void);

void screen_gamestart(void);
void screen_info(void);
void screen_background(void);
void screen_action(void);
void screen_score(void);
void screen_gameover(void);
void screen_gamewin(void);

void joystick_init_menu(void);
void joystick_move_menu(void);
void joystick_init_game(void);
void joystick_move_game(void);

void make_barricade(void);
int move_barricade(void);