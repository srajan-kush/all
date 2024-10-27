section .bss
    inpt_str: resb 100
    temp: resb 1
    count: resb 1

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
mov edi,1
count_loop:

    cmp byte[esi],0
    je end_count

    cmp byte[esi],32
    jne not_inc_count

    inc edi

    not_inc_count:
    inc esi
    jmp count_loop

end_count:

add edi,30h
mov [count],edi

mov eax,4
mov ebx,1
mov ecx,count
mov edx,1
int 80h

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h