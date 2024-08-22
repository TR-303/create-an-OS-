extern main
extern exit

global _start

_start:
	push eax
	push ecx
	call main

	call exit

	hlt