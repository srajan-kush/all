section .bss
	a: resb 3
	b: resb 3
	res: resb 5
section .data
	msg: db 'Enter two Numbers',0Ah
	msglen: equ $-msg
section .text
	global _start
_start:

mov eax,4
mov ebx,1
mov ecx,msg
mov edx,msglen
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

mov ax,0
mov al,byte[a]
sub al,'0'
mov bl,10
mul bl
mov bl,byte[a+1]
sub bl,'0'
add al,bl
mov byte[a],al


mov al,byte[b]
sub al,'0'
mov bl,10
mul bl
mov bl,byte[b+1]
sub bl,'0'
add al,bl
mov byte[b],al

mov al,byte[a]
mov bl,byte[b]
add bl,al

mov dl,0

loop1:
	movzx ax,bl
	mov bl,10
	div bl

	movzx bx,ah
	push bx

	inc dl

	mov bl,al
	sub al,0
	jnz loop1

	mov ecx,res

loop2:
    pop bx
    add bx, '0'
    dec dl
    mov byte[ecx], bl
    inc ecx
    sub dl,0
    jnz loop2

    mov edx, 5
    mov ecx, res
    mov ebx, 1
    mov eax, 4
    int 80h


mov eax,1
xor ebx,ebx
int 80h
