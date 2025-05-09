section .rodata
    f3_add_const1 dd 2.0
    f3_add_const2 dd 6.0
    
section .text

global f3

f3:
    push ebp
    mov ebp, esp
    
    finit
    fld qword[ebp + 8]
    fchs 
    fld dword[f3_add_const1]
    fadd st0, st1
    fld1
    fdivrp st1, st0
    
    fld dword[f3_add_const2]
    faddp st1, st0
    
    
    mov esp, ebp
    pop ebp
    ret

