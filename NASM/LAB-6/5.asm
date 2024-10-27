;5 Create a NASM program to check if a string is a substring of other.

section .bss
    str1: resb 100
    str2: resb 100
    len_str1: resb 1
    len_str2: resb 1
    temp: resb 1
    count: resb 1

section .data
    newline: db 0Ah
    msg1: db 'Given small string is not substring of other'
    len_msg1: equ $-msg1
    msg2: db 'Given small string is substring of other'
    len_msg2: equ $-msg2
    msg3: db 'Enter bigger size string: '
    len_msg3: equ $-msg3
    msg4: db 'Enter smaller size string: '
    len_msg4: equ $-msg4


section .text
global _start
    _start:


pusha
mov eax,4
mov ebx,1
mov ecx,msg3
mov edx,len_msg3
int 80h
popa

mov esi,str1
call std_inpt_str

pusha
mov eax,4
mov ebx,1
mov ecx,msg4
mov edx,len_msg4
int 80h
popa


mov esi,str2
call std_inpt_str



mov esi,str1
mov edi,str2
check_sub_str:

    cmp byte[esi],0
    je not_substring

    mov cl, byte[esi]
    cmp cl,byte[edi]
    je inc_small

    inc esi
    jmp check_sub_str

inc_small:
    mov eax,esi
    inc_small_loop:
        inc eax
        inc edi

        cmp byte[edi],0
        je substring

        mov cl,byte[edi]
        cmp cl,byte[eax]
        je inc_small_loop

        mov edi,str2
        inc esi
        jmp check_sub_str


substring:
    mov eax,4
    mov ebx,1
    mov ecx,msg2
    mov edx,len_msg2
    int 80h
    jmp exit


not_substring:
    mov eax,4
    mov ebx,1
    mov ecx,msg1
    mov edx,len_msg1
    int 80h
    jmp exit

exit:

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h




std_inpt_str:   ; initially make a str pointer in esi
    pusha
    mov edi,0
    inpt_loop:

        pusha
        mov eax,3
        mov ebx,0
        mov ecx,temp
        mov edx,1
        int 80h
        popa

        cmp byte[temp],10
        je end_inpt

        mov eax,[temp]
        mov [esi],eax

        inc esi
        inc edi
        jmp inpt_loop

    end_inpt:
    inc esi
    mov byte[esi],0
    popa
    ret
