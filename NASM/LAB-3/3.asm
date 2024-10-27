section .bss
    temp: resb 1
    option: resb 2
    num1: resw 1
    num2: resw 1
section .data
    msg1: db 'Enter first number: '
    msg_len1: equ $-msg1
    msg2: db 'Enter first number: '
    msg_len2: equ $-msg2
    op_msg: db 'Choose operation:',0Ah,'1.AND',0Ah,'2.OR',0Ah,'3.XOR',0Ah,'your option: '
    op_msg_len: equ $-op_msg

    newline: db 0Ah
section .text
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,msg_len1
int 80h

mov ax,0
input_loop1:

    pusha

    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h

    popa

    cmp byte[temp],10
    je end_input_loop1

    movzx bx,byte[temp]
    sub bx,'0'


    mov cx,10
    mul cx 

    add ax,bx

    jmp input_loop1

end_input_loop1:
mov word[num1],ax


mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,msg_len2
int 80h


mov ax,0
input_loop2:

    pusha

    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h

    popa

    cmp byte[temp],10
    je end_input_loop2

    movzx bx,byte[temp]
    sub bx,'0'


    mov cx,10
    mul cx 

    add ax,bx

    jmp input_loop2

end_input_loop2:
mov word[num2],ax

pusha                       ; pushing all registers into stack


mov eax,4
mov ebx,1
mov ecx,op_msg
mov edx,op_msg_len
int 80h

mov eax,3
mov ebx,0
mov ecx,option
mov edx,2
int 80h


popa

mov ax,word[num1]           ;moving numbers to registers
mov bx,word[num2]


mov cl,byte[option]
sub cl,'0'
cmp cl,1
je and_operation
cmp cl,2
je or_operation

xor ax,bx                   ;performing xor operation
jmp stdoutput

and_operation:
and ax,bx                    ;performing xor operation
jmp stdoutput

or_operation:
or ax,bx


stdoutput:
mov bl,10
mov cl,0
extect_loop:
    div bl

    movzx dx,ah
    add dx,'0'
    push dx

    inc cl
    movzx ax,al

    cmp al,0
    jne extect_loop

print_loop:
    
    pop bx
    mov byte[temp],bl

    pusha

    mov eax,4
    mov ebx,1
    mov ecx,temp
    mov edx,1
    int 80h

    popa

    dec cl
    cmp cl,0
    jne print_loop

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h
