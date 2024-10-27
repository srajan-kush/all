section .bss
    temp: resb 1
section .data:
    msg: db 'Enter a number: ',0Ah
    len: equ $-msg

    divisible: db 'Given number is divisible by 5',0Ah
    divisiblelen: equ $-divisible

    notdivisible: db 'Given number is not divisible by 5',0Ah
    notdivisiblelen: equ $-notdivisible
section .text
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,len
int 80h

read_loop:
    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h

    cmp byte[temp],10
    je end_read_loop
    sub byte[temp],'0'
    movzx ax,byte[temp]
    push ax
    jmp read_loop

end_read_loop:
    pop ax
    cmp ax,0
    je divisible_part
    cmp ax,5
    je divisible_part


notdivisible_part:
    mov eax,4
    mov ebx,1
    mov ecx,notdivisible
    mov edx,notdivisiblelen
    int 80h
    jmp exit


divisible_part:
    mov eax,4
    mov ebx,1
    mov ecx,divisible
    mov edx,divisiblelen
    int 80h
    jmp exit


exit:
mov eax,1
mov ebx,0
int 80h


