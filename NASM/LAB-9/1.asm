section .bss
    temp: resb 1
    str: resb 100
    str_len: resb 1

section .data
    msg1: db 'Enter a string: '
    len1: equ $-msg1
    vowels: db 'aeiouAEIOU0'
    v_len: equ $-vowels

section .text
global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
int 80h

mov esi,str
mov edi,0
str_inpt:

    pusha
    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h
    popa

    cmp byte[temp],10
    je exit_inpt

    mov al,byte[temp]
    mov byte[esi],al
    inc esi
    inc edi
    jmp str_inpt

    exit_inpt:
    mov byte[esi],0
    mov dword[str_len],edi


mov esi,str
remove_1:
    cmp byte[esi],0
    je exit_remove_1

    mov al,byte[esi]
    mov edi,vowels
    remove_2:
        cmp byte[edi],0
        je exit_remove_2

        
        cmp al,byte[edi]
        je below

        inc edi
        jmp remove_2
    
    exit_remove_2:
        mov cl,byte[esi]
        mov byte[temp],cl

        pusha
        mov eax,4
        mov ebx,1
        mov ecx,temp
        mov edx,1
        int 80h
        popa
        below:
        inc esi
        jmp remove_1
exit_remove_1:


mov byte[temp],10
mov eax,4
mov ebx,1
mov ecx,temp
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h
