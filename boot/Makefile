BANANAOSBOOT	=./out/mbr ./out/loader

.PHONY: everything clean all

everything:	$(BANANAOSBOOT)

clean:
	rm -f $(BANANAOSBOOT)

all:	clean everything

./out/mbr:	mbr.asm ./inc/boot.inc ./inc/fat12hdr.inc
	nasm -I ./inc -o $@ $<

./out/loader:	loader.asm ./inc/boot.inc
	nasm -I ./inc -o $@ $<