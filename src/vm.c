#include "vm.h"
#include "opcode.h"

#define debug() { printf("si=%zu pi=%zu m=%zu cli=%zu clc=%zu pri=%zu cpi=%zu clst.pi=%zu clst.si=%zu ", si, pi, m, cli, clc, pri, cpi, cls[cli-1].pi, cls[cli-1].si); }

void growcalls(callframe_t** stack, size_t* cap) {
    callframe_t* tmp = *stack;
    if (!(tmp = (callframe_t*) realloc (tmp, sizeof(callframe_t)* *cap * 2))) {
        fprintf(stderr, "error: oom when growing caps");
        abort();
    }
    *stack = tmp;
    *cap = *cap * 2;
}

void vm_dis(insn_t* pc) {
    switch (pc->op) {
        case VM_OP_NOP: printf("NOP\n"); break;
        case VM_OP_JMP: printf("JMP %u\n", pc->p1); break;
        case VM_OP_RET: printf("RET\n"); break;
        case VM_OP_CHR: printf("CHR %u\n", pc->p1); break;
        case VM_OP_CALL: printf("CALL %u\n", pc->p1); break;
        case VM_OP_FAIL: printf("FAIL\n"); break;
        case VM_OP_CHCE: printf("CHCE %u\n", pc->p1); break;
        case VM_OP_CMT: printf("CMT %u\n", pc->p1); break;
        case VM_OP_ANY: printf("ANY %u\n", pc->p1); break;
        case VM_OP_LOOP: printf("LOOP %u\n", pc->p1); break;
        case VM_OP_BCMT: printf("BCMT %u\n", pc->p1); break;
        case VM_OP_LCAP: printf("LCAP\n"); break;
        case VM_OP_RCAP: printf("RCAP\n"); break;
        case VM_OP_DREF: printf("DREF %u\n", pc->p1); break;
        case VM_OP_BREF: printf("BREF %u\n", pc->p1); break;
        case VM_OP_HREF: printf("HREF %u\n", pc->p1); break;
        case VM_OP_SET: printf("SET %u\n", pc->p1); break;
        case VM_OP_LSET: printf("LSET %u\n", pc->p1); break;
        case VM_OP_CUT: printf("CUT\n"); break;
        case VM_OP_PPSH: printf("PPSH %u\n", pc->p1); break;
        case VM_OP_PPOP: printf("PPOP %u\n", pc->p1); break;
        case VM_OP_RNG: printf("RNG %u %u\n", pc->p1, pc->p2); break;
    }
}

size_t vm_match(
        insn_t*  pc,
        const char* s,
        size_t   start,
        size_t   end,
        cap_t*   cap,
        int*     e) {
    end++;
    cap++;
    size_t m = 1, pi = 0, si = start, cli = 0, clc = 8, pri = 0, cpi = 0;

    callframe_t* cls;
    cls = malloc(sizeof(callframe_t) * clc);
    cls->pi = SIZE_MAX;
    cls->si = SIZE_MAX;
    cls->cli = 0;
    cls->cpi = 0;
    cls->pri = 0;
    cls->cut = 0;
    cli++;

    for (;;) {
        if (!m) {
            if (cli == 0) { free(cls); return 0; }
            if (cls[cli-1].si == SIZE_MAX) {cli--;}
            else {
                pi = cls[cli].pi;
                si = cls[cli].si;
                cpi = cls[cli].cpi;
                pri = cls[pri].pri;
                cli--;
            }
            continue;
        }
        insn_t* insn = &(pc[pi]);
        switch (insn->op) {
            case VM_OP_NOP: {
                pi++;
                debug(); vm_dis(insn);
                continue;
            }
            case VM_OP_JMP: {
                pi += insn->p1;
                debug(); vm_dis(insn);
                continue;
            }
            case VM_OP_RET: {
                pi = cls[--cli].pi;
                debug(); vm_dis(insn);
                if (cli != 0) continue;
                return si-start;
            }
            case VM_OP_CHR: {
                char ch = (byte_t) insn->p1;
                if (ch == s[si]) { si++; pi++; }
                else { m = 0; *e = EWRONGCHR; }
                debug(); vm_dis(insn);
                continue;
            }
            case VM_OP_CALL: {
                if (cli+1 >= clc) growcalls(&cls, &clc);
                cls[cli].pi = pi + 1;
                cls[cli].si = SIZE_MAX;
                cls[cli].pri = SIZE_MAX;
                cls[cli].cpi = SIZE_MAX;
                cls[cli].cut = 0;
                cli++;
                pi += insn->p1;
                debug(); vm_dis(insn);
                continue;
            }
        }
    }
    return 0;
}
