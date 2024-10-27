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

add al,bl


mov bl,10
div bl

mov byte[res],al
add byte[res],'0'
pusha

mov eax,4
mov ebx,1
mov ecx,res
mov edx,1
int 80h

popa
mov byte[res],ah
add byte[res],'0'

mov eax,4
mov ebx,1
mov ecx,res
mov edx,1
int 80h



mov eax,1
mov ebx,0
int 80h


