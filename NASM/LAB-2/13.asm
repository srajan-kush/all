section .bss
    temp: resb 1
    count: resb 1
    res: resb 3
section .data
    msg: db 'Enter a binary number',0Ah
    len: equ $-msg
    newline: db 0Ah
section .data
    global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,len
int 80h

mov byte[count],0
read_loop:
    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h

    cmp byte[temp],10
    je end_read_loop
    movzx ax,byte[temp]
    push ax
    inc byte[count]
    jmp read_loop


end_read_loop:

mov bl,1
mov cl,0
res_loop:
    pop ax
    sub ax,'0'
    mul bl
    add cl,al

    mov al,2
    mul bl
    mov bl,al

    dec byte[count]
    cmp byte[count],0
    jne res_loop



movzx ax,cl
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
