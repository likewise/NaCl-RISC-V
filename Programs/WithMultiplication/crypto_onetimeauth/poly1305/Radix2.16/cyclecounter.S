.text

.globl getcycles
.align 2
getcycles:
#    csrr a1, mcycleh
    csrr a0, mcycle
#    csrr a2, mcycleh
#    bne a1, a2, getcycles
    ret


.globl getsp
.align 2
getsp:
    mv a0, sp
    ret
