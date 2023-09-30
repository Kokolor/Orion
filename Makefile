ARCH = x86_64
TARGET = TahaBoot/data/kernel.elf
SRCDIR = kernel/
OBJDIR = output/

SRCS = $(shell find $(SRCDIR) -type f -name '*.c')
OBJS = $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRCS))

all: $(TARGET)

CC = gcc
LD = ld

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p $(@D)
	$(CC) -ffreestanding -fno-stack-protector -Werror -fno-stack-check -m64 -Ikernel/include/ -ITahaBoot/ -c $< -o $@

$(TARGET): $(OBJS)
	nasm -f elf64 kernel/src/gdt/gdt.asm -o gdt.o
	$(LD) -nostdlib -z max-page-size=0x1000 -Ttext=0x01000000 $^ gdt.o -o $@

image:
	USE_GCC=1 make -C TahaBoot
	make -C TahaBoot disk

run: image
	qemu-system-x86_64 -cpu qemu64,pdpe1gb -bios /usr/share/edk2/x64/OVMF.fd -drive file=TahaBoot/TahaBoot.img,format=raw -m 512M -serial stdio

clean:
	rm -rf $(OBJDIR) $(TARGET) 2>/dev/null || true
	make -C TahaBoot _clean
	make -C TahaBoot clean