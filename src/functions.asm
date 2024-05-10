section .rodata
    fp_constants:
        .neg_two                        dd -2.0
        .neg_one                        dd -1.0
        .two                            dd 2.0
        .six                            dd 6.0
        .fourteen                       dd 14.0


section .text

; /// Calculates ln(x)
; #[cdecl]
; fn f1(value: f32) -> f32
global f1
f1:
    push ebp
    mov ebp, esp
    
    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return ln(2) * log2(value)
    fldln2
    fld dword [ebp+.value]
    fyl2x

    pop ebp
    ret


; /// Calculates (d/dx) ln(x) = 1/x
; #[cdecl]
; fn f1_prime(value: f32) -> f32
global f1_prime
f1_prime:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; reutrn 1 / value
    fld1
    fdiv dword [ebp+.value]

    pop ebp
    ret


; /// Calculates f2(x) = -2x + 14
; #[cdecl]
; fn f2(value: f32) -> f32
global f2
f2:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return -2 * value + 14
    fld dword [ebp+.value]
    fmul dword [fp_constants.neg_two]
    fadd dword [fp_constants.fourteen]

    pop ebp
    ret


; /// Calculates (d/dx) f2(x) = -2
; #[cdecl]
; fn f2_prime(value: f32) -> f32
global f2_prime
f2_prime:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return -2
    fld dword [fp_constants.neg_two]

    pop ebp
    ret


; /// Calculates f3(x) = 1 / (2 - x) + 6
; #[cdecl]
; fn f3(value: f32) -> f32
global f3
f3:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return 1 / (2 - value) + 6
    fld1
    fld dword [fp_constants.two]
    fsub dword [ebp+.value]
    fdivp
    fadd dword [fp_constants.six]

    pop ebp
    ret


; /// Calculates (d/dx) f3(x) = 1 / (2 - x)**2
; #[cdecl]
; fn f3_prime(value: f32) -> f32
global f3_prime
f3_prime:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return 1 / ((2 - value) * (2 - value))
    fld1
    fld dword [fp_constants.two]
    fsub dword [ebp+.value]
    fld dword [fp_constants.two]
    fsub dword [ebp+.value]
    fmulp
    fdivp

    pop ebp
    ret