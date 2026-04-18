# Allow overriding CC and LD for Docker/Standard GCC usage
ASM=nasm
CC?=i686-elf-gcc
LD?=i686-elf-ld

# Compiler flags
CFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -I. -Ikernel -Iquadratic

# Linker flags
LDFLAGS=-nostdlib

# If using standard GCC, we need -m32. If using cross-compiler, it might be implicit or ignored.
# We can detect if CC is "gcc" and add -m32.
ifeq ($(CC),gcc)
    CFLAGS_EXTRA=-m32
    CFLAGS+=$(CFLAGS_EXTRA)
    LDFLAGS+=-m elf_i386
endif

# Kernel object files
KERNEL_OBJS=kernel/kernel.o kernel/screen.o kernel/keyboard.o kernel/math.o kernel/string.o quadratic/quadratic.o

all: quadraticos.iso

quadraticos.iso: kernel.bin
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	echo 'set timeout=0' > isodir/boot/grub/grub.cfg
	echo 'set default=0' >> isodir/boot/grub/grub.cfg
	echo '' >> isodir/boot/grub/grub.cfg
	echo 'menuentry "Quadratic OS" {' >> isodir/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> isodir/boot/grub/grub.cfg
	echo '  boot' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o quadraticos.iso isodir

kernel.bin: boot/boot.o $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -T boot/linker.ld -o kernel.bin boot/boot.o $(KERNEL_OBJS)

boot/boot.o: boot/boot.asm
	$(ASM) -f elf32 $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o *.bin *.iso kernel/*.o quadratic/*.o boot/*.o
	rm -rf isodir

run: quadraticos.iso
	qemu-system-i386 -cdrom quadraticos.iso