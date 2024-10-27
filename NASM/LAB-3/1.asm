section .bss
    a: resb 1
    res: resb 3
section .data:
    msg: db 'Enter a character : ',0Ah
    len: equ $-msg
    assci: db 'Assci Value is :'
    asscilen: equ $-assci
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
mov ecx,a
mov edx,1
int 80h

movzx ax,byte[a]
mov cl,0
mov bl,10
loop:
    div bl
    movzx dx,ah
    add dx,'0'

    push dx
    movzx ax,al

    inc cl
    sub al,0
    jnz loop

mov eax,res
loop1:
    pop bx
    mov byte[eax],bl

    inc eax
    dec cl

    cmp cl,0
    jne loop1


mov eax,4
mov ebx,1
mov ecx,assci
mov edx,asscilen
int 80h

mov eax,4
mov ebx,1
mov ecx,res
mov edx,3
int 80h

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

exit:
mov eax,1
mov ebx,0
int 80h