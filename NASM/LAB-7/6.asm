;1.  Nasm program to Read and print an nxm matrix
section .bss
    j_col resb 2
    arr resw 100    
    colum resb 1
    val resw 1
    i_row resb 2
    bin resb 1
    row resb 1

section .data
    msg2: db 'enter column: '
    len2: equ $ - msg2
    msg1: db 'enter row: '
    len1: equ $ - msg1
    msg3: db 'enter values: '
    len3: equ $ - msg3
    newline: db 0Ah
section .text
global _start
_start:
mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,len1
int 80h

mov eax,3
mov ebx,0
mov ecx,i_row
mov edx,2
int 80h

mov eax,4
mov ebx,1
mov ecx,msg3
mov edx,len3
int 80h

mov eax,3
mov ebx,0
mov ecx,j_col
mov edx,2
int 80h

mov eax,1
mov ebx,0
int 80h

; enter_matrix:

; mov byte[i],1
; mov byte[j],0
; mov esi,arr
; mov eax,0

; outer_loop:
;     mov cl,byte[n]
;     cmp byte[i],cl
;     je outer_exit
;     inner_loop:
;         call input_value
;         mov dx,word[val]
;         mov word[esi+2*eax],dx
;         inc byte[j]
;         inc eax
;         mov cl,byte[m]
;         cmp byte[j],cl
;         jb inner_loop

;         pusha
;         mov eax,4
;         mov ebx,1
;         mov ecx,newline
;         mov edx,1
;         int 80h
;         popa
;         inc byte[i]
;         jmp outer_loop

; outer_exit:
;         ret


; input_value:
;     mov cx,1 
;     mov word[val],0
;     mov dx,0
;     input_loop1:
;     pusha
;         mov eax,4
;         mov ebx,1
;         mov ecx,bin
;         mov edx,1
;         int 80h
;     popa
;     cmp byte[bin],10
;     je input_exit
;         sub byte[bin],30h
;         movzx bx,byte[bin]
;         imul dx,cx
;         add dx,bx
;         imul  cx,10
;         jmp input_loop1
; input_exit:
; mov word[val],dx
;         ret