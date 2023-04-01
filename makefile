# Compiler & linker
ASM           = nasm
LIN           = ld
CC            = gcc
QEMU_IMG      = qemu-img


# Directory
SOURCE_FOLDER = src
OUTPUT_FOLDER = bin
ISO_NAME      = os2023

# Flags
WARNING_CFLAG = -Wall -Wextra -Werror
DEBUG_CFLAG   = -ffreestanding -fshort-wchar -g
STRIP_CFLAG   = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs
CFLAGS        = $(DEBUG_CFLAG) $(WARNING_CFLAG) $(STRIP_CFLAG) -m32 -c -I$(SOURCE_FOLDER)
AFLAGS        = -f elf32 -g -F dwarf
LFLAGS        = -T $(SOURCE_FOLDER)/linker.ld -melf_i386

run: all
	@qemu-system-i386 -s -S -cdrom $(OUTPUT_FOLDER)/$(ISO_NAME).iso
all: build
build: iso
clean:
	rm -rf *.o *.iso $(OUTPUT_FOLDER)/kernel

$(OUTPUT_FOLDER)/%.o: $(SOURCE_FOLDER)/%.c
	@$(CC) $(CFLAGS) $< -o $@

SRC := $(shell find $(SOURCE_FOLDER) -name '*.c')
DIR := $(filter-out src, $(patsubst $(SOURCE_FOLDER)/%, $(OUTPUT_FOLDER)/%, $(shell find $(SOURCE_FOLDER) -type d)))
OBJ := $(patsubst $(SOURCE_FOLDER)/%.c, $(OUTPUT_FOLDER)/%.o, $(SRC))

dir: 
	@for dir in $(DIR); do \
		if [ ! -d $$dir ]; then mkdir -p $$dir; fi \
	done

kernel: $(OBJ)
	@$(ASM) $(AFLAGS) $(SOURCE_FOLDER)/kernel_loader.s -o $(OUTPUT_FOLDER)/kernel_loader.o
	@$(ASM) $(AFLAGS) $(SOURCE_FOLDER)/cpu/interrupt.s -o $(OUTPUT_FOLDER)/interrupt.o

	@$(LIN) $(LFLAGS) $(OBJ) $(OUTPUT_FOLDER)/interrupt.o $(OUTPUT_FOLDER)/kernel_loader.o -o $(OUTPUT_FOLDER)/kernel
	@echo Linking object files and generate elf32...
	@rm -rf ${DIR}
	@rm -f *.o

iso: dir kernel
	@mkdir -p $(OUTPUT_FOLDER)/iso/boot/grub
	@cp $(OUTPUT_FOLDER)/kernel     $(OUTPUT_FOLDER)/iso/boot/
	@cp other/grub1                 $(OUTPUT_FOLDER)/iso/boot/grub/
	@cp $(SOURCE_FOLDER)/menu.lst   $(OUTPUT_FOLDER)/iso/boot/grub/
# TODO: Create ISO image
	@cd $(OUTPUT_FOLDER)/iso && genisoimage -R				\
			-b boot/grub/grub1						        \
			-no-emul-boot              						\
			-boot-load-size 4          						\
			-A os                      						\
			-input-charset utf8        						\
			-quiet                     						\
			-boot-info-table           						\
			-o OS2023.iso              						\
			.
	@cp $(OUTPUT_FOLDER)/iso/OS2023.iso ./bin
	@rm -r $(OUTPUT_FOLDER)/iso/

disk:
	@cd $(OUTPUT_FOLDER) && $(QEMU_IMG) create -f raw drive.img 64m

inserter:
	@$(CC) -Wno-builtin-declaration-mismatch \
		$(SOURCE_FOLDER)/lib/stdmem.c $(SOURCE_FOLDER)/filesystem/fat32.c $(SOURCE_FOLDER)/drivers/cmos.c $(SOURCE_FOLDER)/lib/portio.c \
		other/external-inserter.c \
		-o $(OUTPUT_FOLDER)/inserter
