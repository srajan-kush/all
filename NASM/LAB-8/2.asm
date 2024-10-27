section .bss
    num: resd 2
    temp: resb 1
    arr: resd 100
    m: resb 1
section .data
    space: db ' '
    msg1: db 'Enter size of arr: '
    len1: equ $-msg1
    msg2: db 'Enter your arr:-',0Ah
    len2: equ $-msg2

section .text
global _start
_start:


mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
int 80h

call std_input
mov al,byte[num]
mov byte[m],al

mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,len2
int 80h

call std_input_arr
call std_output_arr


mov eax,1
mov ebx,0
int 80h

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


std_input_arr:
    pusha
    mov esi,arr
    mov edi,0
    mov dl,0
    inpt_arr:

        call std_input
        mov ecx,dword[num]
        mov dword[esi+4*edi],ecx

        inc edi
        inc dl
        cmp dl,byte[m]
        jne inpt_arr
popa
ret

std_output_arr:
mov esi,arr
mov edi,0
mov dl,0
output_arr:

    mov ecx,dword[esi+4*edi]
    mov dword[num],ecx
    call std_output

    pusha
    mov eax,4
    mov ebx,1
    mov ecx,space
    mov edx,1
    int 80h
    popa

    inc edi
    inc dl
    cmp dl,byte[m]
    jne output_arr

popa
ret