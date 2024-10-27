section .bss
	name: resb 32
section .data
	len: equ $name
	dis: db 'your name is '
	disLen: equ $-dis
section .text
global _start
_start:

mov eax,3
mov ebx,0
mov ecx,name
mov edx,len
int 80h

mov eax,4
mov ebx,1
mov ecx,dis
mov edx,disLen
int 80h

mov eax,1


mov eax,4
mov ebx,1
mov ecx,name
mov edx,32
int 80h

mov eax,1
mov ebx,0
int 80h
