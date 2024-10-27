;This code is to read a number(up to 2^16-1) and print it
section .data
    msg1: db "Enter two numbers: ",0Ah
    size1: equ $-msg1
    msg2: db "the sum= "
    size2: equ $-msg2
    newline: db 0Ah

section .bss
    num: resw 1
    sum: resw 1
    digit1: resb 1
    digit0: resb 1
    digit2: resb 1
    temp: resb 1
    counter:resb 1
    count: resb 1


section .text
    global _start
_start:

mov byte[counter],0
mov byte[sum],0

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,size1
int 80h

call read_num

mov ax,word[num]
pusha

call read_num
popa
mov bx,word[num]

add ax,bx
mov word[num],ax 

pusha
mov eax,4
mov ebx,1
mov ecx,msg2
mov edx,size2
int 80h

popa

call print_num

mov eax,1
mov ebx,0
int 80h


read_num:
    pusha
    mov word[num], 0
    loop_read:
        ;; read a digit
        mov eax, 3
        mov ebx, 0
        mov ecx, temp
        mov edx, 1
        int 80h

        cmp byte[temp], 10
        je end_read
        mov ax, word[num]
        mov bx, 10
        mul bx
        mov bl, byte[temp]
        sub bl, 30h
        mov bh, 0
        add ax, bx
        mov word[num], ax
        jmp loop_read
end_read:
    popa
    ret


print_num:
    mov byte[count],0
    pusha
    extract_no:
        cmp word[num], 0
        je print_no
        inc byte[count]
        mov dx, 0
        mov ax, word[num]
        mov bx, 10
        div bx
        push dx
        mov word[num], ax
        jmp extract_no
        print_no:
            cmp byte[count], 0
            je end_print
            dec byte[count]
            pop dx
            mov byte[temp], dl
            add byte[temp], 30h
            mov eax, 4
            mov ebx, 1
            mov ecx, temp
            mov edx, 1
            int 80h
            jmp print_no
            end_print:
                    mov eax,4
                    mov ebx,1
                    mov ecx,newline
                    mov edx,1
                    int 80h
popa
ret