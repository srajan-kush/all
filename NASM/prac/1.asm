section .data
msg1: db "enter the array:", 0Ah
len1: equ $-msg1
msg2: db "the entered array is:", 0Ah
len2: equ $-msg2



section .bss
n: resb 1
temp :  resb 1
array resb 50


section .text
global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
int 80h

mov esi ,array
read_array:
pusha

mov eax,3
mov ebx,0
mov ecx,temp
mov edx,1
int 80h


cmp byte[temp],10
je exit_read_array
mov al,byte[temp]
mov byte[esi],al
inc esi
inc byte[n]
jmp read_array

exit_read_array:
popa


mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,len2
int 80h

mov eax,4
mov ebx,1
mov ecx,array
mov edx,50
int 80h
jmp exit


exit:
mov eax,1
mov ebx,0
int 80h