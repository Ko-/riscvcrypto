.text

.globl getinstret
.align 2
getinstret:
    csrr a1, minstreth
    csrr a0, minstret
    csrr a2, minstreth
    bne a1, a2, getinstret
    ret
.size getinstret,.-getinstret

