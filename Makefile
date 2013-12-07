CC	= /usr/local/cross/bin/x86_64-elf-gcc
LD	= /usr/local/cross/bin/x86_64-elf-ld
CFLAGS =  -std=gnu99 -Iinclude/ -m32 -Wall -Wextra -ffreestanding -nostdlib
OBJCOPY	= /usr/local/cross/bin/x86_64-elf-objcopy
KERNELOBJS = kernel/glue.o kernel/interrupts.o kernel/kmain.o kernel/panic.o kernel/mm.o
DRIVEROBJS = drivers/cpuid.o drivers/pic.o drivers/pit.o drivers/keyboard.o drivers/video.o
LIBRARYOBJS = lib/string.o lib/stdio.o
 
all: disk.iso

disk/kernel.sys: kernel.sys
	cp kernel.sys disk/kernel.sys

disk.iso: disk/kernel.sys
	sudo grub-mkrescue -o disk.iso disk

kernel.sys: $(KERNELOBJS) $(DRIVEROBJS) $(LIBRARYOBJS)
	$(LD) -melf_i386 -nodefaultlibs -Tlinker.ld -o $@ $(KERNELOBJS) $(DRIVEROBJS) $(LIBRARYOBJS)

.s.o:
	nasm -felf $< -o $@

clean:
	@rm -f disk.iso kernel.sys $(KERNELOBJS) $(DRIVEROBJS) $(LIBRARYOBJS)

run: disk.iso
	qemu-system-i386 -cdrom disk.iso -hda hda.img -d int,cpu_reset -s -no-reboot -serial stdio -vga std

again: clean all

install: kernel.sys
	sudo mount /dev/sdb1 flash/
	sudo cp kernel.sys flash/
	sync
	sudo umount /dev/sdb1
	sudo umount /dev/sdb1
