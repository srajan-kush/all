section .bss
    a: resb 3
    b: resb 3
    res: resb 5
section .data:
    msg: db 'Enter length of rectangle(2-digit) : '
    len: equ $-msg
    msg1: db 'Enter width of rectangle(2-digit) : '
    len1: equ $-msg1
    area: db 'Perimeter is :'
    arealen: equ $-area
    newline: equ 0Ah
section .text
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,len
int 80h

mov eax,3
mov ebx,0
mov ecx,a
mov edx,3
int 80h

mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
int 80h

mov eax,3
mov ebx,0
mov ecx,b
mov edx,3
int 80h

mov al,byte[a]
sub al,'0'
mov bl,10
mul bl
movzx bx,byte[a+1]
sub bx,'0'
add ax,bx
mov byte[a],al

mov al,byte[b]
sub al,'0'
mov bl,10
mul bl
movzx bx,byte[b+1]
sub bx,'0'
add ax,bx
mov byte[b],al

mov al,byte[a]
mov bl,byte[b]

add al,bl
mov bl,2

mul bl

;converting assci into characters from ax-register
mov cl,0
mov bl,10
loop:
    div bl
    movzx dx,ah
    add dx,'0'

    push dx
    movzx ax,al

    inc cl
    cmp al,0
    jne loop

mov eax,res
loop1:
    pop bx
    mov byte[eax],bl

    inc eax
    dec cl

    cmp cl,0
    jne loop1


mov eax,4
mov ebx,1
mov ecx,area
mov edx,arealen
int 80h

mov eax,4
mov ebx,1
mov ecx,res
mov edx,5
int 80h

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h


exit:
mov eax,1
mov ebx,0
int 80h
