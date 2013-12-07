#ifndef VIDEO_H
#define VIDEO_H

void init_video(unsigned int* framebuffer, unsigned int width, unsigned int height);

void setchar(unsigned char x, unsigned char y, char c);
void redrawallchars();
void newline();
void kwritechar(char c);
void kprint(char* str);
void scrollup();
void scrolldown();
void tick_cursor();
void draw_cursor();
void erase_cursor();

#endif