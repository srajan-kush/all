section .bss
    a: resb 3
    b: resb 3
    temp: resb 3
section .data:
    msg: db 'Enter two numbers(max-2digit)',0Ah
    len: equ $-msg
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
mov ecx,a
mov edx,3
int 80h

mov eax,3
mov ebx,0
mov ecx,b
mov edx,3
int 80h

mov eax,a
mov ecx,temp
mov dl,2
loop1:
    mov bl,byte[eax]
    mov byte[ecx],bl

    inc eax
    inc ecx

    dec dl
    sub dl,0
    jnz loop1

mov eax,b
mov ecx,a
mov dl,2
loop2:
    mov bl,byte[eax]
    mov byte[ecx],bl

    inc eax
    inc ecx

    dec dl
    sub dl,0
    jnz loop2

mov eax,temp
mov ecx,b
mov dl,2
loop3:
    mov bl,byte[eax]
    mov byte[ecx],bl

    inc eax
    inc ecx

    dec dl
    sub dl,0
    jnz loop3


mov eax,4
mov ebx,1
mov ecx,a
mov edx,3
int 80h

mov eax,4
mov ebx,1
mov ecx,b
mov edx,3
int 80h


mov eax,1
mov ebx,0
int 80h