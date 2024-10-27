section .bss
    array1: resw 50
    len: resw 1
    num: resd 1
    count: resb 1
    temp: resb 1
section .data
 space: db 32
 newline: db 0Ah
 decimal: db '.'

 msg_1: db 'Enter size of an array: '
 msg_len_1: equ $-msg_1
 msg_2: db 'Avarage of given array is: '
 msg_len_2: equ $-msg_2


section .text
  global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg_1
mov edx,msg_len_1
int 80h

call std_input
mov ax,word[num]
mov word[len],ax



mov edi,array1
movzx esi,word[len]
mov word[num],0
dec esi
ReadArray:

    call std_input
    mov ax,word[num]
    mov word[edi],ax

    inc edi
    dec esi
    jns ReadArray

pusha
mov eax,4
mov ebx,1
mov ecx,msg_2
mov edx,msg_len_2
int 80h
popa

mov edi,array1
mov eax,0
movzx esi,word[len]
dec esi
AvgArray:
    add al,byte[edi]
    
    inc edi
    dec esi
    jns AvgArray

mov bl,byte[len]
div bl
mov dword[num],0
mov byte[num],al
call std_output

pusha
mov eax,4
mov ebx,1
mov ecx,decimal
mov edx,1
int 80h
popa

mov al,ah
mov bl,100
mul bl

mov bl,byte[len]
div bl
mov byte[num],al
call std_output


mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

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