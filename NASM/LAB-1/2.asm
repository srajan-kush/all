section .data
	address: db "bamhouri kalan, Palera, Tikamgarh M.P.",0Ah
	len: equ $-address

section .text
	global _start
_start:

mov eax,4
mov ebx,1
mov ecx,address
mov edx,len
int 80h

mov eax,1
mov ebx,0
int 80h




