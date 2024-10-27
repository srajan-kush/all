section .bss
    temp: resb 1
    inpt: resb 2
    shift_size: resb 2
section .data
    msg: db 'Enter a number: '
    msg_len: equ $-msg
    shift_msg: db 'Choose shift operation:',0Ah,'1.left shift',0Ah,'2.Right shift',0Ah,'your option: '
    shift_msg_len: equ $-shift_msg
    shift_size_msg: db 'How many bit you want to shift: '
    shift_size_msg_len: equ $-shift_size_msg

    newline: db 0Ah
section .text
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,msg_len
int 80h

mov ax,0
input_loop:

    pusha

    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h

    popa

    cmp byte[temp],10
    je end_input_loop

    movzx bx,byte[temp]
    sub bx,'0'


    mov cx,10
    mul cx 

    add ax,bx

    jmp input_loop

end_input_loop:                  ; now the final number is in ax-register

pusha                       ; pushing all registers into stack


mov eax,4
mov ebx,1
mov ecx,shift_msg
mov edx,shift_msg_len
int 80h

mov eax,3
mov ebx,0
mov ecx,inpt
mov edx,2
int 80h

mov eax,4
mov ebx,1
mov ecx,shift_size_msg
mov edx,shift_size_msg_len
int 80h

mov eax,3
mov ebx,0
mov ecx,shift_size
mov edx,2
int 80h

popa
mov cl,byte[inpt]
sub cl,'0'
cmp cl,1
je l_shift

mov cl,byte[shift_size]
sub cl,'0'
shr ax,cl      ;using right shift operation
jmp stdoutput


l_shift:
    mov cl,byte[shift_size]
    sub cl,'0'
    shl ax,1                 ;using left shift operation

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
