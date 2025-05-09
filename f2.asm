section .rodata
    f2_mul_const dd -2.0
    f2_add_const dd 14.0

section .text

global f2

f2: 
    push ebp
    mov ebp, esp
    
    FINIT
    FLD qword[ebp + 8] 
    fld dword[f2_mul_const]
    fmulp st1, st0
    fld dword[f2_add_const]
    faddp st1, st0
    
    
    mov esp, ebp
    pop ebp
    ret
