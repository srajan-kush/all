section .bss
    temp: resb 1
    ldigit: resb 1
section .data:
    msg: db 'Enter a number: ',0Ah
    len: equ $-msg
    odd: db 'Number is odd',0Ah
    oddlen: equ $-odd
    even: db 'Number is even',0Ah
    evenlen: equ $-even
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
    je endread_loop
    sub byte[temp],'0'
    movzx ax,byte[temp]
    push ax
    jmp read_loop

endread_loop:
    pop ax
    and al,1
    cmp al,1
    jne even_loop

odd_loop:
    mov eax,4
    mov ebx,1
    mov ecx,odd
    mov edx,oddlen
    int 80h
    jmp exit

even_loop:
    mov eax,4
    mov ebx,1
    mov ecx,even
    mov edx,evenlen
    int 80h
    jmp exit   

exit:
mov eax,1
mov ebx,0
int 80h


