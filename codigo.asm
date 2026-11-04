bits 32

global main
extern printf
extern scanf

section .data
    fmt_print db "%d", 10, 0
    fmt_scan  db "%d", 0

section .bss
    x resd 1
    y resd 1
    i resd 1
    res resd 1
    t1 resd 1
    t2 resd 1
    t3 resd 1

section .text
main:
    push ebp
    mov ebp, esp

    push dword [x]
    call input
    add esp, 4
    push dword [y]
    call input
    add esp, 4
    mov eax, [x]
    cmp eax, 10
    jl ._t1_true
    mov dword [t1], 0
    jmp ._t1_fin
._t1_true:
    mov dword [t1], 1
._t1_fin:
    mov eax, [y]
    cmp eax, 0
    jg ._t2_true
    mov dword [t2], 0
    jmp ._t2_fin
._t2_true:
    mov dword [t2], 1
._t2_fin:
    mov eax, [t1 and t2]
    mov [t3], eax
    mov eax, [t3]
    cmp eax, 0
    je l1
    mov dword [res], 1
    jmp l2
l1:
    mov dword [res], 0
l2:
    push dword [res]
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
