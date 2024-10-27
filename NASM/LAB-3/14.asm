section .bss
    temp: resb 1
    num: resd 1


section .text
    global _start
_start:


call std_input

call std_output


mov eax,1
mov ebx,0
int 80h



std_input:
    pusha
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

        mov ebx,10
        mul ebx

        movzx ebx,byte[temp]
        sub ebx,'0'

        add eax,ebx
        jmp input_loop

    end_input_loop:
    mov dword[num],eax
    popa
    ret

std_output:
    pusha
    mov cx,0
    
    output_loop:
        mov eax,dword[num]
        mov ebx,10
        mov edx,0
        div ebx
        inc cx
        push edx
        mov dword[num],eax
        cmp eax,0
        jne output_loop

    print_loop:
        pop eax

        add al,'0'
        mov byte[temp],al

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



