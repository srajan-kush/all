section .bss
    a: resb 3
    res: resb 10
section .data:
    msg: db 'Enter a number (2-digit number): ',0Ah
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

mov al,byte[a]
sub al,'0'
mov bl,10
mul bl

mov bl,byte[a+1]
sub bl,'0'

add al,bl
mov byte[a],al

movzx bx, byte[a]
mov ax,0

loop:
    add ax,bx
    
    dec bx
    cmp bx,0
    jne loop

mov dl,0
loop1:
    mov bl,10
    div bl
    movzx bx,ah

    push bx

    inc dl

    movzx ax,al
    sub al,0
    jnz loop1

mov eax,res

loop2:
    pop bx
    mov byte[eax],bl
    add byte[eax],'0'
    dec dl
    inc eax

    sub dl,0
    jnz loop2

mov eax,4
mov ebx,1
mov ecx,res
mov edx,10
int 80h

exit:
mov eax,1
mov ebx,0
int 80h


