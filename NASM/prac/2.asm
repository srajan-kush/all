section .bss
    num: resd 2
    temp: resb 1
    m: resb 1
    n: resb 1
    m1: resb 1
    n1: resb 1
    m2: resb 1
    n2: resb 1
    mat1: resd 200
    mat2: resd 200

section .data
    newline: db 0Ah
    space: db 9
    msg1: db 'Enter row of mat: '
    len1: equ $-msg1
    msg2: db 'Enter col of mat: '
    len2: equ $-msg2
    msg3: db 'mat-1:',0Ah
    len3: equ $-msg3
    msg4: db 'mat-2:',0Ah
    len4: equ $-msg4
    msg5: db 'addition of mat:',0Ah
    len5: equ $-msg5

section .text
global _start
_start:

pusha
mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
popa

call std_int_input
mov al,byte[num]
mov byte[m],al

pusha
mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,len2
popa

call std_int_input
mov al,byte[num]
mov byte[n],al


mov esi,mat1
call std_mat_input


mov esi,mat2
call std_mat_input


pusha
mov eax,4
mov ebx,1
mov ecx,msg3
mov edx,len3
popa

mov esi,mat1
call std_mat_output

pusha
mov eax,4
mov ebx,1
mov ecx,msg4
mov edx,len4
popa

mov esi,mat2
call std_mat_output

pusha
mov eax,4
mov ebx,1
mov ecx,msg5
mov edx,len5
popa

mov esi,mat1
mov ebx,mat2
call add_mat

mov eax,1
mov ebx,0
int 80h

std_int_input:
    pusha
    mov eax,0
    std_int_input_loop1:
    
    pusha
    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h
    popa

    cmp byte[temp],10
    je exit_std_int_input_loop1

    mov ebx,10
    mul ebx
    movzx ebx,byte[temp]
    sub ebx,'0'

    add eax,ebx

    jmp std_int_input_loop1

    exit_std_int_input_loop1:
    mov dword[num],eax
    popa
    ret

std_int_output:
    pusha
    mov edi,0
    mov eax,dword[num]
    std_int_output_loop1:

    mov edx,0
    mov ebx,10
    div ebx

    push dx
    inc edi

    cmp eax,0
    jne std_int_output_loop1

    std_out_pop:

        pop dx

        mov byte[temp],dl
        add byte[temp],'0'

        pusha
        mov eax,4
        mov ebx,1
        mov ecx,temp
        mov edx,1
        int 80h
        popa

        dec edi
        cmp edi,0
        jne std_out_pop

    popa
    ret


std_mat_input:  ;initially add of mat in esi, row-> m,column->n
    pusha
    mov cl,byte[m]
    mov dl,byte[n]
    mov edi,0
    std_mat_input_loop1:
        cmp cl,0
        je exit_std_mat_input_loop1

        std_mat_input_loop2:
            cmp dl,0
            je exit_std_mat_input_loop2

            call std_int_input
            mov eax,dword[num]
            mov dword[esi+ edi*4],eax

            dec dl
            inc edi
            jmp std_mat_input_loop2

        exit_std_mat_input_loop2:
        mov dl,byte[n]
        dec cl
        jmp std_mat_input_loop1
    
    exit_std_mat_input_loop1:
    popa
    ret



std_mat_output:  ;initially add of mat in esi, row-> m,column->n
    pusha
    mov cl,byte[m]
    mov dl,byte[n]
    mov edi,0
    std_mat_output_loop1:
        cmp cl,0
        je exit_std_mat_output_loop1

        std_mat_output_loop2:
            cmp dl,0
            je exit_std_mat_output_loop2

            mov eax,dword[esi+ edi*4]
            mov dword[num],eax
            call std_int_output
            
            pusha
            mov eax,4
            mov ebx,1
            mov ecx,space
            mov edx,1
            int 80h
            popa


            dec dl
            inc edi
            jmp std_mat_output_loop2

        exit_std_mat_output_loop2:

        pusha
        mov eax,4
        mov ebx,1
        mov ecx,newline
        mov edx,1
        int 80h
        popa

        mov dl,byte[n]
        dec cl
        jmp std_mat_output_loop1
    
    exit_std_mat_output_loop1:
    popa
    ret     


add_mat:  ;initially add of mat in esi, row-> m,column->n,mat2 in ebx
    pusha
    mov cl,byte[m]
    mov dl,byte[n]
    mov edi,0
    std_mat_add_loop1:
        cmp cl,0
        je exit_std_mat_add_loop1

        std_mat_add_loop2:
            cmp dl,0
            je exit_std_mat_add_loop2

            mov eax,dword[esi+ edi*4]
            add eax,dword[ebx+edi*4]
            mov dword[num],eax
            call std_int_output
            
            pusha
            mov eax,4
            mov ebx,1
            mov ecx,space
            mov edx,1
            int 80h
            popa


            dec dl
            inc edi
            jmp std_mat_add_loop2

        exit_std_mat_add_loop2:

        pusha
        mov eax,4
        mov ebx,1
        mov ecx,newline
        mov edx,1
        int 80h
        popa

        mov dl,byte[n]
        dec cl
        jmp std_mat_add_loop1
    
    exit_std_mat_add_loop1:
    popa
    ret 


