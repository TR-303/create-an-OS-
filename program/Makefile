SRCS	=	${shell find */ -name '*.c'}
TARGETS	=	${SRCS:.c=.elf}

default:	${TARGETS}

%.o: %.c
	gcc -m32 -fno-builtin -nostdlib -nostdinc -fno-stack-protector -no-pie -fno-pic -Wno-implicit-function-declaration -I ../sys/user -c $< -o $@

%.elf: %.o ../crt.a
	ld -m elf_i386 $< ../crt.a -o $@
	@if [ ! -z "${INS}" ]; then \
		elf_name=$$(basename $@); \
		if [ "$$elf_name" = "$(INS)" ]; then \
			echo "Processing ${INS}..."; \
			size=$$(stat -c%s "$@"); \
			padding_size=$$((0x2800 - $$size)); \
			dd if=/dev/zero bs=1 count=$$padding_size >> $@; \
			dd if=$@ of=../disk.img bs=512 seek=1 conv=notrunc; \
		fi; \
	fi
