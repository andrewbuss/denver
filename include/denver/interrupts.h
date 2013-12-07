#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdio.h>
#include <denver/panic.h>
#include <drivers/pic.h>

typedef struct{
   unsigned short base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
   unsigned short sel;                 // Kernel segment selector.
   unsigned char  always0;             // This must always be zero.
   unsigned char  flags;               // More flags. See documentation.
   unsigned short base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed)) idt_entry;

typedef struct{
   unsigned int ds, edi, esi, ebp, esp, ebx, edx, ecx, eax, err_code, eip, cs, eflags, useresp, ss;
} registers;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
typedef struct {
   unsigned short limit;
   unsigned int base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed)) idt_ptr;

extern void idt_flush(unsigned int);
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();
extern void kpanic(unsigned char int_no);
void isr13_handler(registers regs);
void isr14_handler(registers regs);
void isr32_handler();
void isr33_handler();
void isr34_handler(registers regs);
void isr35_handler(registers regs);
void isr36_handler(registers regs);
void isr37_handler(registers regs);
void isr38_handler(registers regs);
void isr39_handler(registers regs);
void isr40_handler(registers regs);
void isr41_handler(registers regs);
void isr42_handler(registers regs);
void isr43_handler(registers regs);
void isr44_handler(registers regs);
void isr45_handler(registers regs);
void isr46_handler(registers regs);
void isr47_handler(registers regs);
void init_idt();
void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);
void isr_handler(registers regs);
#endif