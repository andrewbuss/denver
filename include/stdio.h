#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <string.h>
#include <drivers/video.h>
#include <drivers/serial.h>

#define is_digit(c)	((c) >= '0' && (c) <= '9')

#define ZEROPAD	1
#define SIGN	2		
#define PLUS	4		
#define SPACE	8	
#define LEFT	16		
#define SPECIAL	32	
#define SMALL	64	

#define do_div(n,base) ({int __res; __asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); __res; })

char * number(char * str, int num, int base, int size, int precision,int type);
int atoi(char **s);
char* itoa(char * str, int num, int base, int size, int precision,int type);
unsigned int sprintf(char* buffer, char* fmt, va_list args);
void printf(char *fmt, ...);

#endif