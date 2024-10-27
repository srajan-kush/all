section .bss
	char: resb 1
section .data
	msg: db 'Enter a character',0Ah
	msgLen: equ $-msg
section .text
global _start
_start:
mov eax,4
mov ebx,1
mov ecx,msg
mov edx,msgLen
int 80h

mov eax,3
mov ebx,0
mov ecx,char
mov edx,1
int 80h

mov eax,4
mov ebx,1
mov ecx,char
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h
