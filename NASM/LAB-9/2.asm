section .bss
array:resw 50
num: resw 2
len: resb 1
temp :resb 1

section .data
 msg_1: db 'Enter size of an array: '
 msg_len_1: equ $-msg_1
 msg_2: db 'Given array is: '
 msg_len_2: equ $-msg_2

section .text 
global _start:
_start:


call input_num

call output_num


mov eax,1
mov ebx,0
int 80h

input_num:
mov eax,0

input_num_loop:

pusha 
mov eax,3
mov ebx,0
mov ecx,temp
mov edx,1
int 80h
popa

cmp byte[temp],10
je exit_input_num

mov ebx,10
movzx ecx,byte[temp]
sub ecx,'0'
mul ebx
add eax,ecx

jmp input_num_loop

exit_input_num:
mov dword[num],eax
ret


output_num:
pusha

mov eax,dword[num]
mov dl,0

output_num_loop:
mov bl,10
div bl
movzx cx,ah
push cx
inc dl
cmp al,0
je print_num
movzx ax,al
jmp output_num_loop

print_num:
pop cx
add cx,'0'
mov byte[temp],cl

pusha
mov eax,4
mov ebx,1
mov ecx,temp
mov edx,1
int 80h
popa

dec dl
cmp dl,0
jne print_num

popa 
ret