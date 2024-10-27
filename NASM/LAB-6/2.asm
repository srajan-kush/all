section .bss
    inpt_str: resb 100
    temp: resb 1
    len_inpt: resb 1

section .data
    newline: db 0Ah

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

mov [len_inpt],di
mov esi,inpt_str
pop_loop:

    cmp edi,0
    je end_pop

    pop ax
    mov byte[esi],al

    inc esi
    dec edi
    jmp pop_loop

end_pop:
inc esi
mov byte[esi],0



mov esi,[len_inpt]

mov eax,4
mov ebx,1
mov ecx,inpt_str
mov edx,esi
int 80h

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h