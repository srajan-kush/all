section .bss
	digit: resb 1
	nextline: resb 1
section .data
	msg: db 'Enter a digit',0Ah
	msglen: equ $-msg
	msg1: db 'Your digit is:'
	msg1len: equ $-msg1
section .text
global _start
_start:
mov eax,4
mov ebx,1
mov ecx,msg
mov edx,msglen
int 80h

mov eax,3
mov ebx,0
mov ecx,digit
mov edx,1
int 80h

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,msg1len
int 80h

mov eax,4
mov ebx,1
mov ecx,digit
mov edx,1
int 80h

mov eax,4
mov ebx,1
mov ecx,nextline
mov edx,1
int 80h


mov eax,1
mov ebx,0
int 80h





