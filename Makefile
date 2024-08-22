DIRECTORIES	=	common fs io mem proc sys video
CSRCS	=	${shell find $(DIRECTORIES) -name '*.c' ! -path 'user/utils.c'}
ASMSRCS	=	${shell find $(DIRECTORIES) -name '*.asm'}
COBJS	=	${CSRCS:.c=.o}
ASMOBJS	=	${ASMSRCS:.asm=.oasm}
DEPS	=	${shell find $(DIRECTORIES) -name '*.h'}

ARG_INC = ${addprefix -I ,${DIRECTORIES}}

CRTCSRCS	=	${shell find sys/user -name '*.c'}
CRTASMSRCS	=	${shell find sys/user -name '*.asm'}
CRTCOBJS	=	${CRTCSRCS:.c=.o}
CRTASMOBJS	=	${CRTASMSRCS:.asm=.oasm}

default:
	make -C boot/
	make kernel.elf
	make rewrite_flp
	make crt.a
	make -B -C program/ INS=${INS}

kernel.elf:	${COBJS} ${ASMOBJS}
	ld -T link.ld -m elf_i386 $(COBJS) ${ASMOBJS} -o $@

%.oasm:	%.asm $(DEPS)
	nasm -felf32 $< -o $@

%.o:	%.c $(DEPS)
	gcc -m32 -fno-builtin -nostdlib -nostdinc -fno-stack-protector -no-pie -fno-pic $(ARG_INC) -c $< -o $@

rewrite_flp:
	mcopy -o -i BananaOS.flp kernel.elf ::


crt.a:	${CRTCOBJS} ${CRTASMOBJS}
	ar rcs $@ $(CRTCOBJS) ${CRTASMOBJS}
