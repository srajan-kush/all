section .bss
    temp: resb 1
    count: resb 1
    res: resb 7
section .data:
    msg: db 'Enter a number: '
    len: equ $-msg
    newline: db 0Ah
section .text
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,len
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

mov bl,16
mov ch,0                ; counter 
hexadecimal:
    div bl

    movzx dx,ah

    push dx
    inc ch

    movzx ax,al

    cmp ax,0
    jne hexadecimal

mov byte[count],ch
mov eax,res
number_to_res:
    pop bx

    mov byte[eax],bl

    inc eax
    dec ch

    cmp ch,0
    jne number_to_res


mov ch,byte[count]
mov eax,res

print_loop:

    pusha

    mov cl,byte[eax]

    cmp cl,10
    jb print_single_digit
    jmp print_double_digit

    print_single_digit:
        mov byte[temp],cl
        add byte[temp],'0'

        mov eax,4
        mov ebx,1
        mov ecx,temp
        mov edx,1
        int 80h

    jmp end_print_loop

    print_double_digit:

        add cl,55
        mov byte[temp],cl

        mov eax,4
        mov ebx,1
        mov ecx,temp
        mov edx,1
        int 80h

    end_print_loop:
    popa
    inc eax
    dec ch
    cmp ch,0
    jne print_loop


mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h
