section .bss
    a: resb 2
    b: resb 2
section .data:
    msg: db 'Enter two numbers (1-digit number): ',0Ah
    len: equ $-msg
    greater: db 'first number is greater than second ',0Ah
    greaterlen: equ $-greater
    
    lesser: db 'first number is lesser than second',0Ah
    lesserlen: equ $-lesser

    equal: db 'Both numbers are equal',0Ah
    equallen: equ $-equal
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
je equal_loop
ja greater_loop
jmp lesser_loop


greater_loop:
    mov eax,4
    mov ebx,1
    mov ecx,greater
    mov edx,greaterlen
    int 80h
    jmp exit

lesser_loop:
    mov eax,4
    mov ebx,1
    mov ecx,lesser
    mov edx,lesserlen
    int 80h
    jmp exit   

equal_loop:
    mov eax,4
    mov ebx,1
    mov ecx,equal
    mov edx,equallen
    int 80h
    jmp exit 

exit:
mov eax,1
mov ebx,0
int 80h


