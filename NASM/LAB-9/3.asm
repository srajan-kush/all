section .bss
    temp: resb 1
    str1: resb 100
    str2: resb 100
    str3: resb 200

section .data
    newline: db 0Ah
    msg1: db 'Enter first string: '
    len1: equ $-msg1
    msg2: db 'Enter second string: '
    len2: equ $-msg2
    msg3: db 'concated string is: '
    len3: equ $-msg3

section .text
global _start
_start:


mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
int 80h

mov esi,str1
call std_str_inpt


mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,len2
int 80h

mov esi,str2
call std_str_inpt

call concate

mov eax,4
mov ebx,1
mov ecx,msg3
mov edx,len3
int 80h

mov eax,4
mov ebx,1
mov ecx,str3
mov edx,200
int 80h

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h


std_str_inpt:       ;initially address in esi
    pusha
    str_loop_1:
        pusha
        mov eax,3
        mov ebx,0
        mov ecx,temp
        mov edx,1
        int 80h
        popa

        cmp byte[temp],10
        je exit_str_loop_1

        mov al,byte[temp]
        mov byte[esi],al

        inc esi
        jmp str_loop_1
    exit_str_loop_1:
    mov byte[esi],0
    popa
    ret

concate:
    pusha
    mov esi,str1
    mov edi,str3
    c_l1:
        cmp byte[esi],0
        je exit_c_l1

        mov al,byte[esi]
        mov byte[edi],al
        inc esi
        inc edi
        jmp c_l1

    exit_c_l1:
    mov esi,str2

    c_l2:
    cmp byte[esi],0
    je exit_c_l2 

    mov al,byte[esi]
    mov byte[edi],al

    inc esi
    inc edi
    jmp c_l2

    exit_c_l2:
    mov byte[edi],0
    popa 
    ret
