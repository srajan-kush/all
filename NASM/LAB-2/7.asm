section .bss
    num: resb 4 
    digit: resb 1

section .data
    msg: db 'Enter three digit number', 0Ah
    len: equ $ - msg
    newline: db 0Ah

section .text
    global _start

_start:
    ; Display prompt
    mov eax, 4
    mov ebx, 1
    mov ecx, msg
    mov edx, len
    int 80h

    ; Read input
    mov eax, 3
    mov ebx, 0
    mov ecx, num
    mov edx, 3 
    int 80h


    mov cl,3
    mov eax,num
loop:
    pusha
    mov al,byte[eax]
    mov byte[digit],al

    mov eax, 4
    mov ebx, 1
    mov ecx, digit
    mov edx, 1
    int 80h

    popa
    inc eax
    dec cl
    cmp cl,0
    jne loop

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 80h

exit:
mov eax, 1
mov ebx, 0
int 80h
