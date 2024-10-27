section .data
    vowels db 'aeiouAEIOU', 0            ; List of vowels terminated with null character
    count db 0  
    newline db 10

section .bss
input_string resb 100
temp resb 1

section .text
    global _start
_start:

mov eax,input_string
inpt_loop:
    pusha
    mov eax,3
    mov ebx,0
    mov ecx,temp
    mov edx,1
    int 80h
    popa

    cmp byte[temp],10
    je end_inpt

    mov bl,byte[temp]
    mov byte[eax],bl
    inc eax

    jmp inpt_loop

end_inpt:
    inc eax
    mov byte[eax],0


    mov esi, input_string   ; Load the address of the input string into esi
    mov ecx, 0              ; Initialize ecx to zero (loop counter)
count_vowels:
    mov al, [esi + ecx]     ; Load the current character into al
    cmp al,0              ; Check if we've reached the end of the string
    je end_count            ; If yes, jump to end_count
    mov edi, vowels         ; Load the address of vowels into edi
    mov ebx, 0              ; Initialize ebx to zero (inner loop counter)
check_vowels:
    mov dl, [edi + ebx]     ; Load the current vowel into dl
    cmp dl, 0               ; Check if we've reached the end of the vowels list
    je next_char            ; If yes, jump to next_char
    cmp al, dl              ; Compare the current character with the current vowel
    je increment_count      ; If equal, jump to increment_count
    inc ebx                 ; Increment ebx (move to the next vowel)
    jmp check_vowels        ; Continue checking the next vowel
increment_count:
    inc byte [count]        ; Increment the count of vowels
    jmp next_char           ; Jump to next_char
next_char:
    inc ecx                 ; Increment ecx (move to the next character in the string)
    jmp count_vowels        ; Continue counting vowels
end_count:
    ; The count of vowels is now stored in [count]
    ; Your code to print or use the count goes here (I have written for single digit only)

cmp byte[count],9
ja two_digit

add byte[count], 30h
mov eax,4
mov ebx,1
mov ecx,count
mov edx,1
int 80h
jmp exit

two_digit:
    movzx ax,byte[count]
    mov bl,10

    div bl
    add al,30h
    add ah,30h

    mov byte[count],al
    mov byte[count+1],ah

mov eax,4
mov ebx,1
mov ecx,count
mov edx,1
int 80h
jmp exit


exit:
mov eax,4
mov ebx,1
mov ecx,newline
mov edx,1
int 80h
    ; Exit the program

mov eax, 1              ; Exit system call number
xor ebx, ebx            ; Exit status (0 for success)
int 80h                 ; Call kernel