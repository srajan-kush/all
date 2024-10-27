
;single digit addition

section .bss
	a: resb 2
	b: resb 2
 	c: resb 2


section .text
	global _start
_start:
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

add al,'0'
mov byte[c],al

mov eax,4
mov ebx,1
mov ecx,c
mov edx,2
int 80h

mov eax,1
xor ebx,ebx
int 80h
















