#include <denver/mm.h>
#include <stdio.h>

unsigned int* pagedirectory;

void init_mm(unsigned int* _pagedirectory){
	pagedirectory = _pagedirectory;
}

void four_meg_identity_map(unsigned int addr){
	
	//printf("Identity mapping address 0x%08X\n",addr);
	pagedirectory[addr>>22]=(addr&0xFFC00000)|0x83;
	asm volatile("invlpg (%0)":: "b" (addr));
}