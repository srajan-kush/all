section .bss
 temp: resb 1
 num: resw 1
section .text
    global _start
    _start:

    mov ax,256
    mov bx,6

    div bx

    mov dx,0

    div bx

    add al,'0'
    mov byte[temp],al

    mov eax,4
    mov ebx,1
    mov ecx,temp
    mov edx,1
    int 80h

mov eax,1
mov ebx,0
int 80h