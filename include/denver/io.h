#ifndef IO_H
#define IO_H

static inline void outb( unsigned short port, unsigned char val ){
    asm volatile( "outb %0, %1" :: "a"(val), "Nd"(port));
}
static inline unsigned char inb( unsigned short port ){
    unsigned char ret;
    asm volatile( "inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static inline void io_wait( void ){
    asm volatile( "outb %%al, $0x80" : : "a"(0));
}

#endif