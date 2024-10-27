section .bss
    a: resb 3
    b: resb 3
    res: resb 5
section .data:
    msg: db 'Enter two 2-digit numbers',0Ah
    len: equ $-msg
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

movzx ax,byte[a]
movzx bx,byte[b]

add ax,bx
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

mov eax,1
mov ebx,0
int 80h