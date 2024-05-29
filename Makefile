DIRECTORIES = kernel video common mem int process
SRCS = $(shell find $(DIRECTORIES) -name '*.c')
SRCS += $(shell find $(DIRECTORIES) -name '*.asm')
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.asm=.o)
DEPS = $(shell find $(DIRECTORIES) -name '*.h')

ARG_INC = $(addprefix -I ,$(DIRECTORIES))

kernel.elf: $(OBJS)
	ld -T kernel/link.ld -m elf_i386 $(OBJS) -o $@
	mcopy -o -i boot.flp kernel.elf ::

%_asm.o: %_asm.asm $(DEPS)
	nasm -felf32 $< -o $@

%.o: %.c $(DEPS)
	gcc -m32 -fno-builtin -nostdlib -nostdinc -fno-stack-protector -no-pie -fno-pic $(ARG_INC) -c $< -o $@
