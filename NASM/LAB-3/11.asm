section .data
msg1: db "enter 2 numbers:", 0Ah
len1: equ $-msg1
msg2: db "1st Number is greater ",0Ah
len2: equ $-msg2
msg3: db "2nd Number is greater ",0Ah
len3: equ $-msg3

section .bss
d: resb 2;
e: resb 2;

section .text
global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
int 80h

mov eax,3
mov ebx,0
mov ecx,d
mov edx,2
int 80h

mov eax,3
mov ebx,0
mov ecx,e
mov edx,2
int 80h

mov al,byte[d];
sub al,'0'
mov bl,byte[e];
sub bl,'0'


cmp al,bl
ja first


second:
mov eax,4
mov ebx,1
mov ecx,msg3
mov edx,len3
int 80h
jmp exit


first:
mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,len2
int 80h
jmp exit


exit:
mov eax,1
mov ebx,0
int 80h