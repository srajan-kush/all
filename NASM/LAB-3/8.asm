section .bss
    temp: resb 1
    num: resw 1
    func: resb 1 

section .data
    msg: db 'Enter a number: '
    msg_len: equ $-msg
    space: db ' '
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
mov bx,10
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

    mul bx
    movzx cx,byte[temp]
    sub cx,'0'

    add ax,cx

    jmp input_loop

end_input_loop:
mov dl,al             ; final input number is in dl

mov bx,0
mov cx,1
fabonacci_start:
    add bx,cx
    mov ax,bx
    mov bx,cx
    mov cx,ax
    mov word[num],ax

    pusha

    pusha
    mov eax,4
    mov ebx,1
    mov ecx,space
    mov edx,1
    int 80h
    popa

    stdout:
        mov ax,word[num]
        mov cl,0
        mov bl,10
        loop:
            div bl
            movzx dx,ah
            add dx,'0'

            push dx
            movzx ax,al

            inc cl
            sub al,0
            jnz loop

        loop1:
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
            jne loop1

    popa

    dec dl
    cmp dl,0
    jne fabonacci_start

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h


