section .bss
    a: resb 2
section .data
    msg: db 'Enter a character',0Ah
    msglen: equ $-msg

    upper: db 'character is in upperCase',0Ah
    upperlen: equ $-upper

    lower: db 'character is in lowerCase',0Ah
    lowerlen: equ $-lower

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
mov ecx,a
mov edx,2
int 80h

mov al,byte[a]

cmp al,97
jb uppercase
jmp lowercase

uppercase:
    mov eax,4
    mov ebx,1
    mov ecx,upper
    mov edx,upperlen
    int 80h
    jmp exit

lowercase:
    mov eax,4
    mov ebx,1
    mov ecx,lower
    mov edx,lowerlen
    int 80h
    jmp exit

exit:
    mov eax,1
    mov ebx,0
    int 80h

