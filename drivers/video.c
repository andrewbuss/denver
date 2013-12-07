#include <drivers/video.h>
#include "font.h"

unsigned char cellbuffer[256][176];
unsigned char buffertop = 0;
unsigned int* framebuffer;
unsigned int fb_width;
unsigned int fb_height;
unsigned char cx=1, cy;
unsigned int vscroll = 0;
unsigned char cursor_state = 0;

void init_video(unsigned int* fb_addr, unsigned int width, unsigned int height){
	framebuffer = fb_addr;
	fb_width = width;
	fb_height = height;
}

void drawchar(unsigned char x, unsigned char y, char c){
	if(x>=175) return;
	if(y>=86) return;
	unsigned int color = 0xFFCCBF99;
	for(int yi=0;yi<12;yi++){
		if(font[c][yi]&0x1) framebuffer[8*x+(y*12+yi+4)*fb_width+7] = color;
		else framebuffer[8*x+(y*12+yi+4)*fb_width+7] = 0x00000000;
		if(font[c][yi]&0x2) framebuffer[8*x+(y*12+yi+4)*fb_width+6] = color;
		else framebuffer[8*x+(y*12+yi+4)*fb_width+6] = 0x00000000;
		if(font[c][yi]&0x4) framebuffer[8*x+(y*12+yi+4)*fb_width+5] = color;
		else framebuffer[8*x+(y*12+yi+4)*fb_width+5] = 0x00000000;
		if(font[c][yi]&0x8) framebuffer[8*x+(y*12+yi+4)*fb_width+4] = color;
		else framebuffer[8*x+(y*12+yi+4)*fb_width+4] = 0x00000000;
		if(font[c][yi]&0x10) framebuffer[8*x+(y*12+yi+4)*fb_width+3] = color;
		else framebuffer[8*x+(y*12+yi+4)*fb_width+3] = 0x00000000;
		if(font[c][yi]&0x20) framebuffer[8*x+(y*12+yi+4)*fb_width+2] = color;
		else framebuffer[8*x+(y*12+yi+4)*fb_width+2] = 0x00000000;
		if(font[c][yi]&0x40) framebuffer[8*x+(y*12+yi+4)*fb_width+1] = color;
		else framebuffer[8*x+(y*12+yi+4)*fb_width+1] = 0x00000000;
		if(font[c][yi]&0x80) framebuffer[8*x+(y*12+yi+4)*fb_width] = color;
		else framebuffer[8*x+(y*12+yi+4)*fb_width] = 0x00000000;
	}
}

void redrawallchars(){
	for(unsigned char y=0; y<87; y++) for(unsigned char x=0;x<175;x++){
		drawchar(x,y,cellbuffer[(unsigned char)(buffertop+y+vscroll)][x]);
	}
}
void setchar(unsigned char x, unsigned char y, char c){
	cellbuffer[(unsigned char)(y+buffertop)][x] = c;
	drawchar(x,y-vscroll,c);
}

void newline(){
	cx = 1;
	if(cy>=255){
		for(int i=0; i<176; i++) cellbuffer[buffertop][i] = 0;
		buffertop++;
		redrawallchars();
	}
	else cy++;
}
void kwritechar(char c){
	erase_cursor();
	if(c == '\b'){
		if(cx>1){
			cx--;
			setchar(cx,cy,' ');
		}
	}
	else if(c == '\n') newline();
	else if(cx >= 175){
		newline();
		kwritechar(c);
	}
	else setchar(cx++,cy,c);
	if(cy>vscroll+85){
		vscroll=cy-85;
		redrawallchars();
	}
	cursor_state = 1;
	draw_cursor();
}

void scrollup(){
	if(vscroll>0){
		vscroll--;
		redrawallchars();
	}
}

void scrolldown(){
	if(vscroll<200){
		vscroll++;
		redrawallchars();
	}
}
void kprint(char* str){
	while(*str) kwritechar(*(str++));
}

void draw_cursor(){
	if(vscroll>cy)return;
	if(cy-vscroll>=86)return;
	for(int x=cx*8;x<cx*8+8;x++){
		framebuffer[((cy-vscroll)*12+10+4)*fb_width+x] = 0xFFCCBF99;
		framebuffer[((cy-vscroll)*12+11+4)*fb_width+x] = 0xFFCCBF99;
	}
	cursor_state = 1;
}

void erase_cursor(){
	if(vscroll>cy)return;
	if(cy-vscroll>=86)return;
	for(int x=cx*8;x<cx*8+8;x++){
		framebuffer[((cy-vscroll)*12+10+4)*fb_width+x] = 0;
		framebuffer[((cy-vscroll)*12+11+4)*fb_width+x] = 0;
	}
	cursor_state = 0;
}

void tick_cursor(){
	if(cursor_state) erase_cursor();
	else draw_cursor();
}