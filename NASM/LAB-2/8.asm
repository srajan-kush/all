section .bss
    a: resb 2
    b: resb 2
    res: resb 3
section .data:
    msg: db 'Enter two one digit numbers',0Ah
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
mov edx,2
int 80h

mov eax,3
mov ebx,0
mov ecx,b
mov edx,2
int 80h

mov al,byte[a]
mov bl,byte[b]

sub al,'0'
sub bl,'0'

cmp al,bl
ja first

sub bl,al
add bl,'0'
mov byte[res],bl
jmp exit

first:
    sub al,bl
    add al,'0'
    mov byte[res],al


exit:
mov eax,4
mov ebx,1
mov ecx,res
mov edx,1
int 80h


mov eax,1
mov ebx,0
int 80h


