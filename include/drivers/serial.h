#ifndef SERIAL_H
#define SERIAL_H 

#include <denver/io.h>

void init_serial();
int serial_received(); 
char read_serial();
int serial_transmit_empty();
void write_serial(char a);
void print_serial(char* s);
void printf_serial(char *fmt, ...);

#endif