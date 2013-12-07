#include <denver/panic.h>

void panic(unsigned char int_no){
	printf("Kernel Panic: Exception 0x%02X ",int_no);
	switch(int_no){
		case 0: printf("Division by Zero"); break;
		case 2: printf("Non-Maskable Interrupt"); break;
		case 4: printf("Overflow"); break;
		case 5: printf("Bounds Check"); break;
		case 6: printf("Invalid Opcode"); break;
		case 7: printf("Device not Available"); break;
		case 8: printf("Double Fault"); break;
		case 9: printf("Coprocessor Segment Overrun"); break;
		case 10: printf("Invalid Task State Segment"); break;
		case 11: printf("Segment Not Present"); break;
		case 12: printf("Stack Fault Exception"); break;
		case 13: printf("General Protection Fault"); break;
		case 14: printf("Page Fault"); break;
		case 15: printf("Unassigned Interrupt"); break;
		case 16: printf("x87 FPU Exception"); break;
		case 17: printf("Alignment Check"); break;
		case 18: printf("Machine Check"); break;
		case 19: printf("SIMD FPU Exception"); break;
	}
	printf(" ");
	asm volatile("cli");
	for(;;){}
}