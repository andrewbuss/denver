#include <drivers/cpuid.h>
#include <denver/interrupts.h>
#include <drivers/pic.h>
#include <drivers/keyboard.h>
#include <drivers/pit.h>
#include <drivers/serial.h>
#include <denver/mm.h>
#include <stdio.h>

unsigned int* colorbuffer;
void* fb_addr;
void* fb_end;
unsigned int fb_pitch;
unsigned int fb_width;
unsigned int fb_height;
unsigned char fb_bpp;
unsigned char fb_type;

int kmain(void* mbinfo, unsigned int magic, unsigned int* pagedirectory){
	printf("Magic number: 0x%08X\n", magic);
	printf("Page directory located at: 0x%08X\n", pagedirectory);
	printf("Initializing memory management\n");
	init_mm(pagedirectory);
	printf("Multiboot structure at 0x%08X, total size: %u bytes\n",mbinfo,*((unsigned int*)mbinfo));
	void* tags_end = mbinfo+*((unsigned int*)mbinfo);
	mbinfo += 8;
	while(mbinfo<tags_end){
		void* tag_start = mbinfo;
		unsigned int tag_type = *((unsigned int*)mbinfo);
		if(!tag_type) break;
		unsigned int tag_size = *((unsigned int*)(mbinfo+4));
		mbinfo+=8;
		switch(tag_type){
		case 1:
			printf("Command line: %s\n",(char*)mbinfo);
			break;
		case 2:	
			printf("Bootloader name: %s\n",(char*)mbinfo);
			break;
		case 4:
			printf("Free: %u KB lower memory, %u KB upper memory\n",*((unsigned int*)mbinfo),*((unsigned int*)(mbinfo+4)));
			break;
		case 5:
			printf("Booted from device 0x%08X, partition 0x%08X, subpartition 0x%08X\n",*((unsigned int*)mbinfo),*((unsigned int*)(mbinfo+4)),*((unsigned int*)(mbinfo+8)));
			break;
		case 6:	{
				unsigned int entry_size = *((unsigned int*)mbinfo);
				mbinfo += 4;
				printf("Memory map - each entry is version %u and has %u bytes\n", *(unsigned int*)mbinfo, entry_size);
				mbinfo += 4;
				while((unsigned int)mbinfo<(unsigned int)tag_start+tag_size){
					unsigned long chunk_start = *((unsigned long*)mbinfo);
					unsigned long chunk_len = *((unsigned long*)(mbinfo+8));
					unsigned long chunk_end = chunk_start+chunk_len-1;
					unsigned int chunk_type = *((unsigned int*)(mbinfo+16));
					printf("Entry: %016lX - %016lX, type %u ",chunk_start,chunk_end,chunk_type);
					switch(chunk_type){
					case 1: printf("(available)\n"); break;
					case 3: printf("(available, ACPI info)\n"); break;
					case 4: printf("(hibernation-preserved)\n"); break;
					default: printf("(reserved)\n"); break;
					}
					mbinfo+=entry_size;
				}
			}
			break;
		case 7:{
				unsigned short vbe_mode =  *((unsigned short*)(mbinfo));
				unsigned short vbe_interface_seg = *((unsigned short*)(mbinfo+2));
				unsigned short vbe_interface_off = *((unsigned short*)(mbinfo+4));
				unsigned short vbe_interface_len = *((unsigned short*)(mbinfo+6));
				unsigned char* vbe_control_info = (unsigned short*)(mbinfo+8);
				unsigned char* vbe_control_info_end = (unsigned short*)(mbinfo+520);
				unsigned char* vbe_mode_info = (unsigned short*)(mbinfo+520);
				unsigned char* vbe_mode_info_end = (unsigned short*)(mbinfo+776);
				printf("VBE mode 0x%08X, interface segment 0x%08X, interface offset 0x%08X, interface length 0x%08X\n",
					vbe_mode,vbe_interface_seg,vbe_interface_off,vbe_interface_len);
				printf("VBE control info at 0x%08X, VBE mode info at 0x%08X\n",vbe_control_info,vbe_mode_info);
			}
			break;
		case 8:
			fb_addr = *((void**)mbinfo);
			colorbuffer = (unsigned int*) fb_addr;
			fb_pitch = *((unsigned int*)(mbinfo+8));
			fb_width = *((unsigned int*)(mbinfo+12));
			fb_height = *((unsigned int*)(mbinfo+16));
			fb_bpp = *((unsigned char*)(mbinfo+20));
			fb_type = *((unsigned char*)(mbinfo+21));
			mbinfo+=22;	
			printf("Framebuffer 0x%016X, pitch %u bytes, resolution %ux%u, %u bpp, type %u\n",fb_addr,fb_pitch,fb_width,fb_height,fb_bpp,fb_type);
			fb_end = (void*)(fb_addr+fb_pitch*fb_height);
			for(unsigned int i=fb_addr;i<=fb_end;i+=0x00400000) four_meg_identity_map(i);
			switch(fb_type){
			case 0:
				printf("Palette (%u colors): ",*((unsigned int*)mbinfo));
				while((unsigned int)mbinfo<(unsigned int)tag_start+tag_size){
					unsigned char r = *((unsigned char*)mbinfo++);
					unsigned char g = *((unsigned char*)mbinfo++);
					unsigned char b = *((unsigned char*)mbinfo++);
					printf("%02X%02X%02X ",r,g,b);
				}
				printf("\n");
			}
			init_video(fb_addr,fb_width,fb_height);
			redrawallchars();
			break;
		case 9:
			//printf("ELF section headers, ignoring\n");
			break;
		case 10:
			//printf("APM info, ignoring\n");
			break;
		default:
			printf("Unknown tag type %u, size %u, starts at 0x%08X. Contents:\n",tag_type,tag_size,tag_start);
			while((unsigned int)mbinfo<(unsigned int)tag_start+tag_size){
				printf("%c",*((unsigned char*)mbinfo++));
			}
			printf("\n");
			break;
		}
		mbinfo=tag_start+tag_size;
		if((unsigned int)mbinfo%8) mbinfo+=(8-(unsigned int)mbinfo%8);
	}
	printf("0x%08X : 0x%08X\n",fb_addr, pagedirectory[(unsigned int)fb_addr>>22]);
	printf("Initializing PIC and IDT\n");
	init_pic();
	setmask(0xFFFC);
	init_idt();
	printf("Initializing timer to 10 Hz\n");
	init_timer(100);
	asm volatile("sti;");
	for(;;){}
	return 0xC0DED1ED;
}
