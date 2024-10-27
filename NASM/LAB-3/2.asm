section .bss
num: resb 5
temp: resb 1
count: resb 1

section .data
 msg: db 'Enter a Number: '
 msglen: equ $-msg
 newline: db 0Ah
 nonprime: db 'Given number is Non-Prime',0Ah
 nonprimelen: equ $-nonprime
 prime: db 'Given number is Prime',0Ah
 primelen: equ $-prime

section .text
global _start
    _start:


mov eax,4
mov ebx,1
mov ecx,msg
mov edx,msglen
int 80h

mov eax,num
mov dl,0
read_loop:
    pusha
    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h

    cmp byte[temp],10
    je end_read_loop

    popa
    mov bl,byte[temp]
    sub bl,'0'
    mov byte[eax],bl
    inc eax
    inc dl
    jmp read_loop

end_read_loop:
popa


mov ecx,num
mov al,0
convert_integer:
    mov bl,10
    mul bl
    add al,byte[ecx]
    inc ecx
    dec dl
    cmp dl,0
    jne convert_integer



mov bl,al
mov byte[temp],al
sub bl,1
primeFind_loop:
    div bl
    cmp ah,0
    je printnonPrime

    cmp bl,2
    je printPrime

    movzx ax,byte[temp]
    sub bl,1
    jmp primeFind_loop


printnonPrime:
    mov eax,4
    mov ebx,1
    mov ecx,nonprime
    mov edx,nonprimelen
    int 80h
    jmp exit


printPrime:
    mov eax,4
    mov ebx,1
    mov ecx,prime
    mov edx,primelen
    int 80h
    jmp exit



exit:
mov eax,1
mov ebx,0
int 80h


;;subprogram to print a number
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
;;The memory location ’newline’ should be declared with the ASCII key for
;;new line in section.data.
popa
ret




