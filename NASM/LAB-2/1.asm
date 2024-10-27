section .bss
    num: resb 3
section .data
    msg: db 'Enter two digit number',0Ah
    len: equ $-msg
    newline: db 0Ah
section .text
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,len
int 80h

mov eax,3
mov ebx,0
mov ecx,num
mov edx,3
int 80h

mov al,byte[num]
sub al,'0'
mov bl,10
mul bl
mov bl,byte[num+1]
sub bl,'0'
add al,bl

mov byte[num],al


mov al,byte[num]
movzx ax,al
mov bl,10

div bl

add al,'0'
add ah,'0'

mov byte[num],al
mov byte[num+1],ah

mov eax,4
mov ebx,1
mov ecx,num
mov edx,2
int 80h

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h
