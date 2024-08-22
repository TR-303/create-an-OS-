TARGET_FLP	=	../BananaOS.flp

default:	mbr.bin loader.bin rewrite_flp

mbr.bin:	src/mbr.asm inc/boot.inc inc/fat12hdr.inc
	nasm -I ./inc -o $@ $<

loader.bin:	src/loader.asm inc/boot.inc
	nasm -I ./inc -o $@ $<

rewrite_flp:
	dd if=mbr.bin of=${TARGET_FLP} bs=512 count=1 conv=notrunc
	mcopy -o -i ${TARGET_FLP} loader.bin ::
