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
; fn df1dx(value: f32) -> f32
global df1dx
df1dx:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; reutrn 1.0 / value
    fld1
    fdiv dword [ebp+.value]

    pop ebp
    ret


; /// Calculates (d2/dx2) ln(x) = -1 / x**2
; #[cdecl]
; fn d2f1dx2(value: f32) -> f32
global d2f1dx2
d2f1dx2:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return -1.0 / (x * x)
    fld dword [fp_constants.neg_one]
    fld dword [ebp+.value]
    fmul dword [ebp+.value]
    fdivp

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
; fn df2dx(value: f32) -> f32
global df2dx
df2dx:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return -2
    fld dword [fp_constants.neg_two]

    pop ebp
    ret


; /// Calculates (d2/dx2) d2(x) = 0
global d2f2dx2
d2f2dx2:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return 0.0
    fldz

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

    ; return 1.0 / (2.0 - value) + 6.0
    fld1
    fld dword [fp_constants.two]
    fsub dword [ebp+.value]
    fdivp
    fadd dword [fp_constants.six]

    pop ebp
    ret


; /// Calculates (d/dx) f3(x) = 1 / (2 - x)**2
; #[cdecl]
; fn df3dx(value: f32) -> f32
global df3dx
df3dx:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; return 1.0 / ((2.0 - value) * (2.0 - value))
    fld1
    fld dword [fp_constants.two]
    fsub dword [ebp+.value]
    fld dword [fp_constants.two]
    fsub dword [ebp+.value]
    fmulp
    fdivp

    pop ebp
    ret


; /// Calculates (d2/dx2) f3(x) = 2 / (2 - x)**3
global d2f3dx2
d2f3dx2:
    push ebp
    mov ebp, esp

    .argbase                            equ 8
    .value                              equ .argbase+0

    ; value = 2 - value
    fld dword [fp_constants.two]
    fsub dword [ebp+.value]
    fstp dword [ebp+.value]

    ; return 2.0 / (value * value * value)
    fld dword [fp_constants.two]
    fld dword [ebp+.value]
    fmul dword [ebp+.value]
    fmul dword [ebp+.value]
    fdivp

    pop ebp
    ret