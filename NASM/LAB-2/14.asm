section .bss
    temp: resb 1
    count: resb 1
section .data:
    msg: db 'Enter a number: '
    len: equ $-msg
    out: db 'Reverse number is: '
    out_len: equ $-out
    newline: db 0Ah
section .text
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,len
int 80h


mov byte[count],0
read_loop:
    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h

    cmp byte[temp],10
    je end_read_loop

    movzx ax,byte[temp]
    inc byte[count]
    push ax
    jmp read_loop

end_read_loop:

mov eax,4
mov ebx,1
mov ecx,out
mov edx,out_len
int 80h

print_loop:
    pop ax
    mov byte[temp],al

    mov eax,4
    mov ebx,1
    mov ecx,temp
    mov edx,1
    int 80h

    dec byte[count]
    cmp byte[count],0
    jne print_loop

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h


mov eax,1
mov ebx,0
int 80h