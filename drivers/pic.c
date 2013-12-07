#include <drivers/pic.h>
#include <stdio.h>

void init_pic(){
	outb(0x20, 0x11);
	io_wait();
	outb(0xA0, 0x11);
	io_wait();
	outb(0x21, 0x20);
	io_wait();
	outb(0xA1, 0x28);
	io_wait();
	outb(0x21, 4);
	io_wait();
	outb(0xA1, 2);
	io_wait();
	outb(0x21, 0x01);
	io_wait();
	outb(0xA1, 0x01);
	io_wait();
	outb(0x21, 0xFF);
	io_wait();
}

void mask_IRQ(unsigned char irq) {
    unsigned short port;
    unsigned char value;
    if(irq < 8) port = PIC1_DATA;
    else{
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);       
    io_wait(); 
    
}
 
void unmask_irq(unsigned char irq) {
    unsigned short port;
    unsigned char value;
    if(irq < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);    
    io_wait();  
    
}

void setmask(unsigned short mask){
	int i = 0;
	while(i<16){
		if(mask%2) mask_IRQ(i);
		else unmask_irq(i);
		io_wait();
		mask >>= 1;
		i += 1;
	}
}

void send_eoi(unsigned char irq){
	if(irq >= 8) outb(PIC2_COMMAND,PIC_EOI); 
	outb(PIC1_COMMAND,PIC_EOI);
	io_wait();
}