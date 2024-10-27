section .bss
    temp: resw 1
    num: resd 1
    matrix: resd 200
    m: resw 1
    n: resw 1
    i: resw 1
    j: resw 1

section .data
    tab: db 9
    newline: db 0Ah
    msg1: db 'Enter number of rows: '
    len_msg1: equ $-msg1
    msg2: db 'Enter number of coloumns: '
    len_msg2: equ $-msg2
    msg3: db "Enter the elements one by one(row by row) : ",0Ah
    len_msg3: equ $-msg3
    msg4: db 'Sum of all elements is: '
    len_msg4: equ $-msg4


section .text
global _start
    _start:

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len_msg1
int 80h

call std_inpt
mov ax,[num]
mov [m],ax

mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,len_msg2
int 80h

call std_inpt
mov ax,[num]
mov [n],ax

mov eax,4
mov ebx,1
mov ecx,msg3
mov edx,len_msg3
int 80h

call matrix_inpt

call sum_of_all

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

exit:
mov eax,1
mov ebx,0
int 80h

std_inpt:
    pusha
    mov edx,0
    mov eax,0
    std_inpt_loop:
        
        pusha
        mov eax,3
        mov ebx,0
        mov ecx,temp
        mov edx,1
        int 80h
        popa

        cmp byte[temp],10
        je end_inpt

        mov ebx,10
        mul ebx
        movzx ebx,word[temp]
        sub ebx,'0'
        add eax,ebx

        jmp std_inpt_loop

    end_inpt:
    mov [num],eax
    popa
    ret


std_output:
    pusha
    mov eax,[num]
    mov esi,0

    std_output_loop:

        cmp eax,0
        je end_output_loop

        mov edx,0
        mov ebx,10
        div ebx

        push edx
        inc esi
        jmp std_output_loop
    
    end_output_loop:

        pop eax

        add ax,'0'
        mov [temp],ax

        pusha
        mov eax,4
        mov ebx,1
        mov ecx,temp
        mov edx,1
        int 80h
        popa

        dec esi
        cmp esi,0
        jne end_output_loop

    popa
    ret



matrix_inpt:
    pusha
    mov esi,matrix
    mov edi,0
    mov word[i],0
    inpt_i_loop:
        mov word[j],0
        inpt_j_loop:
            call std_inpt
            mov eax,[num]

            mov [esi + 4*edi],eax

            add edi,1
            inc word[j]
            mov ax,word[n]
            cmp word[j],ax
            jne inpt_j_loop

        inc word[i]
        mov ax,word[m]
        cmp [i],ax
        jne inpt_i_loop

    popa
    ret

sum_of_all:
    pusha
    mov esi,matrix
    mov edi,0
    mov word[i],0
    mov ecx,0
    out_i_loop:
        mov word[j],0
        out_j_loop:
            mov eax,[esi + 4*edi]

            add ecx,eax
            add edi,1
            inc word[j]
            mov ax,word[n]
            cmp word[j],ax
            jne out_j_loop

        inc word[i]
        mov ax,word[m]
        cmp [i],ax
        jne out_i_loop
    
    pusha
    mov eax,4
    mov ebx,1
    mov ecx,msg4
    mov edx,len_msg4
    int 80h
    popa

    mov [num],ecx
    call std_output
    popa
    ret