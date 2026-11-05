bits 32

global main
extern printf
extern scanf

section .data
    fmt_print db "%d", 10, 0
    fmt_scan  db "%d", 0

section .bss
<<<<<<< HEAD
    x resd 1
    y resd 1
    res resd 1
    push x
    call input
    add esp, 4
    push y
    call input
    add esp, 4
    t1 resd 1
    mov eax, [x]
    cmp eax, [10]
    jl %_lt_t1
    mov dword [t1], 0
    jmp %_end_lt_t1
%_lt_t1:
    mov dword [t1], 1
%_end_lt_t1:
    t2 resd 1
    mov eax, [y]
    cmp eax, [0]
    jg %_gt_t2
    mov dword [t2], 0
    jmp %_end_gt_t2
%_gt_t2:
    mov dword [t2], 1
%_end_gt_t2:
    t3 resd 1
    mov eax, [t1]
    and eax, [t2]
    mov [t3], eax
    mov eax, [t3]
    cmp eax, 0
    je l1
    mov eax, [1]
    mov [res], eax
    jmp l2
l1:
    mov eax, [0]
    mov [res], eax
l2:
    push dword [res]
    call output
    add esp, 4
=======
    push_dir resd 1
    b resd 1
    a resd 1
    t1 resd 1
    t2 resd 1
>>>>>>> 0d13e2d360bb01cbac9ff8a5e01d73312fac9992

section .text
main:
    push ebp
    mov ebp, esp

<<<<<<< HEAD
=======
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
>>>>>>> 0d13e2d360bb01cbac9ff8a5e01d73312fac9992

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
