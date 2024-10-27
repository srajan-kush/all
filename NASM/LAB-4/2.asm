section .data
 str: db 'Enter a single digit number',0Ah
 len: equ $-str
 fact: db 'factorial is : '
 len1: equ $-fact
 newline: db 0Ah

section .bss
 temp: resb 1
 num : resw 2

section .text
 global _start
    _start:

mov eax,4
mov ebx,1
mov ecx,str
mov edx,len
int 80h



call read_num

call factorial

mov eax,4
mov ebx,1
mov ecx,fact
mov edx,len1
int 80h

call print_num


mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h

factorial:

    pusha

    mov ax,1
    mov bl,byte[num]
    fact_loop:
        mul bl

        dec bl
        cmp bl,0
        jne fact_loop
    
    mov word[num],ax

    popa
    ret





read_num:
    mov word[num],0
    pusha
    read_loop:
        mov eax,3
        mov ebx,0
        mov ecx,temp
        mov edx,1
        int 80h

    cmp byte[temp],10
    je end_read
    mov ax,word[num]
    movzx bx,byte[temp]
    sub bx,'0'

    mov cx,10
    imul ax,cx
    add ax,bx
    mov word[num],ax
    jmp read_loop

    end_read:
        popa
        ret


print_num:
    pusha
    mov ax,word[num]
    mov bl,10
    mov dl,0
    loop:
        div bl
        movzx cx,ah
        push cx
        inc dl
        movzx ax,al
        cmp al,0
        jne loop
    
    poploop:
        pop cx
        add cx,'0'
        mov byte[temp],cl
        
        pusha
        mov eax,4
        mov ebx,1
        mov ecx,temp
        mov edx,1
        int 80h

        popa
        dec dl
        cmp dl,0
        jne poploop

    end_print:
    popa
    ret








