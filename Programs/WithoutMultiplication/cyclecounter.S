.text

.globl getcycles
.align 2
getcycles:
    csrr t0, mcycle
    sw   t0, 0(a0)
    csrr t0, mhpmcounter3
    sw   t0, 4(a0)
    csrr t0, mhpmcounter4
    sw   t0, 8(a0)
    ret

.globl icachemisses
.type icachemisses, @function
icachemisses:
    li t0, 0x6001
    csrw mhpmevent3, t0 #Branch direction mis
    li t0, 0x102
    csrw mhpmevent4, t0 #Branch target miss
    ret

.globl getsp
getsp:
    mv a0, sp
    ret

.globl getra
getra:
    lw a0, 1244(sp)
    ret
