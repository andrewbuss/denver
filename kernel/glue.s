extern kmain
extern printf
global stage3
global premain
KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)
FB_PAGE_NUMBER equ (0xFC000000 >> 22)

section .data
align 0x1000

PageDirectory:
	dd 0x00000083
    times (KERNEL_PAGE_NUMBER - 1) dd 0
    dd 0x00000083
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

GDT:
	dd 0
	dd 0
	dd 0x0000FFFF
	dd 0x00CF9A00
	dd 0x0000FFFF
	dd 0x00CF9200

GDTR:
	dw 24
	dd GDT

endedmessage db 10,'Kernel has terminated with value 0x%X',10,0
section .text
align 8

mbhdr:
	dd 0xE85250D6
	dd 0
	dd (mbhdr_end-mbhdr)
	dd  -(0xE85250D6 + mbhdr_end - mbhdr) 

	dw 5,0
	dd 20
	dd 1400
	dd 1050
	dd 32

	dd 0
	dd 0
mbhdr_end:


stage3:	
	mov ecx, (PageDirectory - KERNEL_VIRTUAL_BASE)
    mov cr3, ecx
    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx
    lea ecx, [premain]
    jmp ecx 
premain:
	lgdt [GDTR]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:.flush
.flush:
    mov esp, stack+0x4000
    push PageDirectory
    push eax
    push ebx
	call kmain
ended:
	push eax
	push endedmessage
	call printf
	cli
	hlt

global idt_flush
extern panic

%macro ISR_PREHANDLE 0
	pushad
	mov eax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
%endmacro

%macro ISR_POSTHANDLE 0
	pop eax	
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	popad
	iret
%endmacro

%macro ISR_PANIC 1
	global isr%1
	isr%1:
		cli
		push %1
		call panic
%endmacro

%macro ISR 1
	global isr%1
	extern isr%1_handler
	isr%1:
		cli
		ISR_PREHANDLE
		call isr%1_handler
		ISR_POSTHANDLE
%endmacro

%macro ISR_NOHANDLER 1
	global isr%1
	isr%1:
		iret
%endmacro


idt_flush:
	mov eax, [esp+4]
	lidt [eax]
	ret

ISR_PANIC 0
ISR_NOHANDLER 1
ISR_PANIC 2
ISR_NOHANDLER 3
ISR_PANIC 4
ISR_PANIC 5
ISR_PANIC 6
ISR_PANIC 7
ISR_PANIC 8
ISR_PANIC 9
ISR_PANIC 10
ISR_PANIC 11
ISR_PANIC 12
ISR 13
ISR 14
ISR_NOHANDLER 15
ISR_PANIC 16
ISR_PANIC 17
ISR_PANIC 18
ISR_PANIC 19
ISR_PANIC 20
ISR_PANIC 21
ISR_PANIC 22
ISR_PANIC 23
ISR_PANIC 24
ISR_PANIC 25
ISR_PANIC 26
ISR_PANIC 27
ISR_PANIC 28
ISR_PANIC 29
ISR_PANIC 30
ISR_PANIC 31
ISR 32
ISR 33
ISR 34
ISR 35
ISR 36
ISR 37
ISR 38
ISR 39
ISR 40
ISR 41
ISR 42
ISR 43
ISR 44
ISR 45
ISR 46
ISR 47

section .bss
align 32
stack:
    resb 0x4000