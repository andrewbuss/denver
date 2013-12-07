#include <denver/interrupts.h>
#include <stdio.h>
#include <drivers/video.h>
#include <drivers/keyboard.h>

idt_entry idt_entries[256];
idt_ptr   idt;

void init_idt(){
	idt.limit = sizeof(idt_entry) * 256 -1;
	idt.base  = (unsigned int)&idt_entries;
	for(unsigned char* i=(unsigned char*)idt_entries;i<((unsigned char*)&(idt_entries[256]));i++){*i = 0;}
	idt_set_gate(0, (unsigned int)isr0, 0x08, 0x8E);
	idt_set_gate(1, (unsigned int)isr1, 0x08, 0x8E);
	idt_set_gate(2, (unsigned int)isr2, 0x08, 0x8E);
	idt_set_gate(3, (unsigned int)isr3, 0x08, 0x8E);
	idt_set_gate(4, (unsigned int)isr4, 0x08, 0x8E);
	idt_set_gate(5, (unsigned int)isr5, 0x08, 0x8E);
	idt_set_gate(6, (unsigned int)isr6, 0x08, 0x8E);
	idt_set_gate(7, (unsigned int)isr7, 0x08, 0x8E);
	idt_set_gate(8, (unsigned int)isr8, 0x08, 0x8E);
	idt_set_gate(9, (unsigned int)isr9, 0x08, 0x8E);
	idt_set_gate(10, (unsigned int)isr10, 0x08, 0x8E);
	idt_set_gate(11, (unsigned int)isr11, 0x08, 0x8E);
	idt_set_gate(12, (unsigned int)isr12, 0x08, 0x8E);
	idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8E);
	idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8E);
	idt_set_gate(15, (unsigned int)isr15, 0x08, 0x8E);
	idt_set_gate(16, (unsigned int)isr16, 0x08, 0x8E);
	idt_set_gate(17, (unsigned int)isr17, 0x08, 0x8E);
	idt_set_gate(18, (unsigned int)isr18, 0x08, 0x8E);
	idt_set_gate(19, (unsigned int)isr19, 0x08, 0x8E);
	idt_set_gate(20, (unsigned int)isr20, 0x08, 0x8E);
	idt_set_gate(21, (unsigned int)isr21, 0x08, 0x8E);
	idt_set_gate(22, (unsigned int)isr22, 0x08, 0x8E);
	idt_set_gate(23, (unsigned int)isr23, 0x08, 0x8E);
	idt_set_gate(24, (unsigned int)isr24, 0x08, 0x8E);
	idt_set_gate(25, (unsigned int)isr25, 0x08, 0x8E);
	idt_set_gate(26, (unsigned int)isr26, 0x08, 0x8E);
	idt_set_gate(27, (unsigned int)isr27, 0x08, 0x8E);
	idt_set_gate(28, (unsigned int)isr28, 0x08, 0x8E);
	idt_set_gate(29, (unsigned int)isr29, 0x08, 0x8E);
	idt_set_gate(30, (unsigned int)isr30, 0x08, 0x8E);
	idt_set_gate(31, (unsigned int)isr31, 0x08, 0x8E);
	idt_set_gate(32, (unsigned int)isr32, 0x08, 0x8E);
	idt_set_gate(33, (unsigned int)isr33, 0x08, 0x8E);
	idt_set_gate(34, (unsigned int)isr34, 0x08, 0x8E);
	idt_set_gate(35, (unsigned int)isr35, 0x08, 0x8E);
	idt_set_gate(36, (unsigned int)isr36, 0x08, 0x8E);
	idt_set_gate(37, (unsigned int)isr37, 0x08, 0x8E);
	idt_set_gate(38, (unsigned int)isr38, 0x08, 0x8E);
	idt_set_gate(39, (unsigned int)isr39, 0x08, 0x8E);
	idt_set_gate(40, (unsigned int)isr40, 0x08, 0x8E);
	idt_set_gate(41, (unsigned int)isr41, 0x08, 0x8E);
	idt_set_gate(42, (unsigned int)isr42, 0x08, 0x8E);
	idt_set_gate(43, (unsigned int)isr43, 0x08, 0x8E);
	idt_set_gate(44, (unsigned int)isr44, 0x08, 0x8E);
	idt_set_gate(45, (unsigned int)isr45, 0x08, 0x8E);
	idt_set_gate(46, (unsigned int)isr46, 0x08, 0x8E);
	idt_set_gate(47, (unsigned int)isr47, 0x08, 0x8E);
	idt_flush((unsigned int)&idt);
}

void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags){
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
	idt_entries[num].sel     = sel;
	idt_entries[num].always0 = 0;
	idt_entries[num].flags   = flags /* | 0x60 */;
}

void isr13_handler(registers regs){
	printf("General protection fault! Error number is 0x%08X\n",regs.err_code);
	panic(13);
}
void isr14_handler(registers regs){
	printf("Page fault! Can't do anything about that now. Error number is 0x%08X\n",regs.err_code);
	panic(14);
}
void isr32_handler(){
	unsigned static long int ticks = 0;
	ticks++;
	if(!(ticks%60)) tick_cursor();
	send_eoi(0);
}

void isr33_handler(){
	process_scancode();
	send_eoi(1);
}
void isr34_handler(registers regs){}
void isr35_handler(registers regs){}
void isr36_handler(registers regs){}
void isr37_handler(registers regs){}
void isr38_handler(registers regs){}
void isr39_handler(registers regs){}
void isr40_handler(registers regs){}
void isr41_handler(registers regs){}
void isr42_handler(registers regs){}
void isr43_handler(registers regs){}
void isr44_handler(registers regs){}
void isr45_handler(registers regs){}
void isr46_handler(registers regs){}
void isr47_handler(registers regs){}