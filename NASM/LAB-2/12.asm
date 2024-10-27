section .data
    msg: db 'Enter two numbers:',0Ah
    len: equ $-msg
    newline: db 0Ah

section .bss
    a: resb 3
    b: resb 3
    res: resb 3

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

mov eax,3
mov ebx,0
mov ecx,b
mov edx,3
int 80h

mov al, byte [a]
sub al, '0'
mov bl, 10
mul bl
movzx bx, byte [a + 1]
sub bx, '0'
add ax, bx
mov word [a], ax

mov al, byte [b]
sub al, '0'
mov bl, 10
mul bl
movzx bx, byte [b + 1]
sub bx, '0'
add ax, bx
mov word [b], ax


mov ax,word[a]
mov bl,byte[b]

cmp al,bl
jb swap
jmp gcd_loop

swap:
    mov ax,word[b]
    mov bl,byte[a]


gcd_loop:
    mov byte[res],bl
    div bl
    movzx bx,ah
    push bx
    mov ax,word[a]
    cmp bl,0
    jne gcd_loop


movzx ax,byte[res]
mov dl,0
loop:
    mov bl,10
    div bl

    movzx cx,ah
    push cx

    inc dl

    movzx ax,al
    cmp al,0
    jne loop


mov eax,res

loop1:
    pop cx
    mov byte[eax],cl
    add byte[eax],'0'
    inc eax
    dec dl
    cmp dl,0
    jne loop1


mov eax,4
mov ebx,1
mov ecx,res
mov edx,3
int 80h

mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h

mov eax,1
mov ebx,0
int 80h