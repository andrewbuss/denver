#ifndef PIC_H
#define PIC_H

#include <denver/io.h>

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)
#define PIC_EOI 0x20
#define PIC_READ_IRR 0x0A
#define PIC_READ_ISR 0x0B

void init_pic();
void mask_IRQ(unsigned char irq);
void unmask_IRQ(unsigned char irq);
void setmask(unsigned short mask);
void send_eoi(unsigned char irq);

#endif