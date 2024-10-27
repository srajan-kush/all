section .bss
    a: resb 3
    res: resb 5
    num: resd 1
    temp: resb 1
section .data:
    msg: db 'Enter a number(1-digit) : '
    len: equ $-msg
    area: db 'Factorial is :'
    arealen: equ $-area
    newline: db 0Ah
section .text
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,len
int 80h

mov eax,3
mov ebx,0
mov ecx,a
mov edx,2
int 80h

mov al,byte[a]
sub al,'0'
mov byte[a],al



movzx eax,byte[a]
movzx ebx,byte[a]
dec ebx

loop3:
    mul ebx
    dec ebx
    cmp ebx,1
    jne loop3

mov dword[num],eax

call std_output

mov eax,1
mov ebx,0
int 80h




std_input:
mov eax,0
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

    movzx ebx,byte[temp]
    sub ebx,'0'

    mov ecx,10
    mul ecx
    add eax,ebx

    jmp input_loop

end_input_loop:
mov dword[num],eax
ret

std_output:
    pusha
    mov cx,0
    out_loop:
        mov edx,0
        mov eax,dword[num]
        mov ebx,10
        div ebx
        inc cx

        push edx

        mov dword[num],eax
        cmp eax,0
        jne out_loop

    print_loop:
        pop ebx
        add bx,'0'
        mov byte[temp],bl
        
        pusha
        mov eax,4
        mov ebx,1
        mov ecx,temp
        mov edx,1
        int 80h
        popa

        dec cx
        cmp cx,0
        jne print_loop

    popa
    ret