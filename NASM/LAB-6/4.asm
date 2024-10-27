section .bss
    inpt_str: resb 100
    temp: resb 1

section .data
    newline: db 0Ah
    msg_1: db 'Enter a string: '
    len_msg_1: equ $-msg_1
    msg_2: db 'String is palindrome'
    len_msg_2: equ $-msg_2
    msg_3: db 'String is not palindrome'
    len_msg_3: equ $-msg_3

section .text
global _start
    _start:

mov esi,inpt_str
inpt_loop:

    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h

    cmp byte[temp],10
    je end_inpt

    mov al,byte[temp]
    mov byte[esi],al

    inc esi
    jmp inpt_loop

end_inpt:
inc esi
mov byte[esi],0


mov esi,inpt_str
mov edi,0
push_loop:

    cmp byte[esi],0
    je end_push

    movzx ax,byte[esi]
    push ax
    
    inc edi
    inc esi
    
    jmp push_loop

end_push:

mov esi,inpt_str
pop_loop:

    cmp edi,0
    je end_pop

    pop ax
    cmp byte[esi],al
    jne not_palindrome

    inc esi
    dec edi
    jmp pop_loop

end_pop:

mov eax,4
mov ebx,1
mov ecx,msg_2
mov edx,len_msg_2
int 80h

jmp exit


not_palindrome:
mov eax,4
mov ebx,1
mov ecx,msg_3
mov edx,len_msg_3
int 80h


exit:
mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h