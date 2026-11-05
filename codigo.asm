bits 32

global main
extern printf
extern scanf

section .data
    fmt_print db "%d", 10, 0
    fmt_scan  db "%d", 0

section .bss
    push_dir resd 1
    b resd 1
    a resd 1
    t1 resd 1
    t2 resd 1

section .text
main:
    push ebp
    mov ebp, esp

    push b
    call input
    add esp, 4
    mov eax, 1
    add eax, 2
    mov [t1], eax
    mov eax, [t1]
    add eax, [b]
    mov [t2], eax
    mov eax, [t2]
    mov [a], eax
    push dword [a]
    call output
    add esp, 4

    mov esp, ebp
    pop ebp
    xor eax, eax
    ret

global output
output:
    push ebp
    mov ebp, esp
    push dword [ebp+8]
    push fmt_print
    call printf
    add esp, 8
    mov esp, ebp
    pop ebp
    ret

global input
input:
    push ebp
    mov ebp, esp
    push dword [ebp+8]
    push fmt_scan
    call scanf
    add esp, 8
    mov esp, ebp
    pop ebp
    ret
