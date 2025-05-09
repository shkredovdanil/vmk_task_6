section .text

global f1

f1: ; ln(x) = log2(x) * ln(2)
    push ebp
    mov ebp, esp

    FINIT
    FLD qword [ebp + 8]     ; st(0) = x
    FLDLN2                  ; st(0) = ln(2), st(1) = x
    FXCH                    ; st(0) = x, st(1) = ln(2)
    FYL2X                   ; st(0) = ln(x)

    mov esp, ebp
    pop ebp
    ret
