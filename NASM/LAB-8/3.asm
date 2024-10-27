section .bss
    num: resd 2
    temp: resb 1
    matrix: resd 200
    m: resw 2
    n: resw 2

section .data
    space: db 9
    newline: db 0Ah
    msg1: db 'Enter size row of matrix: '
    len1: equ $-msg1
    msg2: db 'Enter size col of matrix: '
    len2: equ $-msg2
    msg3: db 'Enter MATRIX values with enter saparated:',0Ah
    len3: equ $-msg3
section .text
global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
int 80h

call std_input
mov ax,word[num]
mov word[m],ax

mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,len2
int 80h

call std_input
mov ax,word[num]
mov word[n],ax

mov eax,4
mov ebx,1
mov ecx,msg3
mov edx,len3
int 80h

mov cx,word[m]
mov dx,word[n]
mov edi,0
mov esi,matrix
inpt_mat_loop1:
    cmp cx,0
    je exit_inpt_mat_loop1

    inpt_loop2:
        cmp dx,0
        je exit_inpt_mat_loop2

        call std_input
        mov eax,dword[num]
        mov dword[esi+4*edi],eax

        inc edi
        dec dx
        jmp inpt_loop2

        exit_inpt_mat_loop2:
        dec cx
        mov dx,word[n]
        jmp inpt_mat_loop1

    exit_inpt_mat_loop1:


mov cx,word[m]
mov dx,word[n]
mov edi,0
mov esi,matrix
out_mat_loop1:
    cmp cx,0
    je exit_out_mat_loop1

    out_loop2:
        cmp dx,0
        je exit_out_mat_loop2

        mov eax,dword[esi+4*edi]
        mov dword[num],eax
        call std_output

        pusha
        mov eax,4
        mov ebx,1
        mov ecx,space
        mov edx,1
        int 80h
        popa

        inc edi
        dec dx
        jmp out_loop2

        exit_out_mat_loop2:

        pusha
        mov eax,4
        mov ebx,1
        mov ecx,newline
        mov edx,1
        int 80h
        popa

        dec cx
        mov dx,word[n]
        jmp out_mat_loop1

    exit_out_mat_loop1:




mov eax,1
mov ebx,0
int 80h









std_output:
    pusha
    mov eax,dword[num]
    mov ebx,10
    mov esi,0
    std_out_loop:
    mov edx,0
    div ebx

    push dx

    inc esi
    cmp eax,0
    jne std_out_loop

    pop_loop:
    pop bx
    add bl,'0'
    mov byte[temp],bl

    pusha
    mov eax,4
    mov ebx,1
    mov ecx,temp
    mov edx,1
    int 80h
    popa

    dec esi
    jnz pop_loop

popa
ret


std_input:
    pusha
    mov edx,0
    mov eax,0
    std_input_loop:
    
    pusha 
    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h
    popa

    cmp byte[temp],10
    je exit_inpt

    mov ebx,10
    mul ebx
    movzx cx,byte[temp]
    sub cx,'0'
    movzx ecx,cx

    add eax,ecx

    jmp std_input_loop

exit_inpt:
mov dword[num],eax
popa 
ret