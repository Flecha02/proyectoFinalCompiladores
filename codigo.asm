bits 32

global main
extern printf
extern scanf

section .data
    fmt_print db "%d", 10, 0
    fmt_scan  db "%d", 0

section .bss
    push_dir resd 1
    x resd 1
    y resd 1
    z resd 1
    l1 resd 1
    l2 resd 1
    t1 resd 1
    t2 resd 1
    t3 resd 1

section .text
main:
    push ebp
    mov ebp, esp

    push x
    call input
    add esp, 4
    push y
    call input
    add esp, 4
    mov eax, 1
    add eax, [x]
    mov [t1], eax
    mov eax, [t1]
    mov [z], eax
    mov eax, [x]
    cmp eax, [y]
    jl ._t2_true
    mov dword [t2], 0
    jmp ._t2_fin
._t2_true:
    mov dword [t2], 1
._t2_fin:
    mov eax, [t2]
    cmp eax, 0
    je l1
    mov eax, [x]
    add eax, [y]
    mov [t3], eax
    mov eax, [t3]
    mov [z], eax
    jmp l2
l1:
    mov dword [z], 0
l2:
    push dword [z]
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
