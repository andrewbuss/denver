#include <drivers/serial.h>
#include <stdio.h>

void init_serial() {
   outb(0x3F8 + 1, 0x00);    // Disable all interrupts
   outb(0x3F8 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(0x3F8 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(0x3F8 + 1, 0x00);    //                  (hi byte)
   outb(0x3F8 + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(0x3F8 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(0x3F8 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   printf("Initialized serial port\n");
}

int serial_received() {
   return inb(0x3F8 + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
   return inb(0x3F8);
}

int serial_transmit_empty() {
   return inb(0x3F8 + 5) & 0x20;
}

void write_serial(char a) {
   while (serial_transmit_empty() == 0);
   outb(0x3F8,a);
}

void print_serial(char* s){
	while(*s){
		write_serial(*(s++));
	}
}

void printf_serial(char *fmt, ...){
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	sprintf(buffer, fmt, args);
	print_serial(buffer);
	va_end(args);
}