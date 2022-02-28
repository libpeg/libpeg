#pragma once

enum {
    VM_OP_NOP  = 0,
    VM_OP_JMP  = 1,
    VM_OP_RET  = 2,
    VM_OP_CHR  = 3,
    VM_OP_CALL = 4,
    VM_OP_FAIL = 5,
    VM_OP_CHCE = 6,
    VM_OP_CMT  = 7,
    VM_OP_ANY  = 8,
    VM_OP_LOOP = 9,
    VM_OP_BCMT = 10,
    VM_OP_LCAP = 11,
    VM_OP_RCAP = 12,
    VM_OP_DCAP = 13,
    VM_OP_DREF = 14,
    VM_OP_BREF = 15,
    VM_OP_HREF = 16,
    VM_OP_SET  = 17,
    VM_OP_LSET = 18,
    VM_OP_CUT  = 19,
    VM_OP_PPSH = 20,
    VM_OP_PPOP = 21
};
