#include <drivers/keyboard.h>
#include <drivers/video.h>

#define LEFT_CTRL_DOWN 0x1
#define LEFT_ALT_DOWN 0x2
#define LEFT_SHIFT_DOWN 0x4
#define RIGHT_ALT_DOWN 0x8
#define RIGHT_CTRL_DOWN 0xF
#define RIGHT_SHIFT_DOWN  0x10

unsigned char scancodestack[6];
unsigned char modifiers = 0;
unsigned char scsi = 0;

const unsigned char characters[256]={
	0,  27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', '\b', '\t',
	'q', 'w', 'e', 'r',  't', 'y', 'u', 'i', 
	'o', 'p', '[', ']', '\n', 0,  'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
 	'\'', '`',   0,	'\\', 'z', 'x', 'c', 'v', 
 	'b', 'n', 'm', ',', '.', '/',   0,  '*',
	0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
const unsigned char shiftcharacters[256]={
	0,27,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S',
	'D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V',
	'B','N','M','<','>','?',0,0,0,' ',0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void reset_cpu(){
    unsigned char temp = 0x02;
    while (temp & 0x02) temp = inb(0x64);
    outb(0x64, 0xFE);
}

void process_keycode(unsigned short sc){
	switch(sc){
	case 0x0313:
		reset_cpu();
		break;
	default:
		if(characters[sc%256]=='u'){
			scrollup();
		}else if(characters[sc%256]=='n'){
			scrolldown();
		}else if((sc>>8)&LEFT_SHIFT_DOWN){
			printf("%c",shiftcharacters[sc%256]);
		}else{
			printf("%c",characters[sc%256]);
		}
	}
}

void process_scancode(){
	unsigned char sc = inb(0x60);
	if(scsi){
		scancodestack[scsi++] = sc;
		if(scancodestack[0] == 0xE0){
			switch(sc){
				break;
			case 0x1D:
				modifiers ^= RIGHT_CTRL_DOWN;
				break;
			case 0x38:
				modifiers ^= RIGHT_ALT_DOWN;
				break;
				scsi = 0;
			default:

			scsi = 0;
			}
		}
	}else{
		switch(sc){
		case 0x38:
		case 0xB8:
			modifiers ^= LEFT_ALT_DOWN;
			break;
		case 0x2A:
		case 0xAA:
			modifiers ^= LEFT_SHIFT_DOWN;
			break;
		case 0x1D:
		case 0x9D:
			modifiers ^= LEFT_CTRL_DOWN;
			break;
		case 0xE0:
			scancodestack[scsi++] = 0xE0;
			break;
		default:
			process_keycode((modifiers << 8) | sc);
		}
	}
}