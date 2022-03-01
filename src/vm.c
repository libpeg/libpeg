#include "vm.h"
#include "opcode.h"

#define vm_inspect() { printf("si=%zu pi=%zu m=%zu cli=%zu clc=%zu pri=%zu cpi=%zu clst.pi=%zu clst.si=%zu ", si, pi, m, cli, clc, pri, cpi, cls[cli-1].pi, cls[cli-1].si); }

void growcalls(callframe_t** stack, size_t* cap) {
    callframe_t* tmp = *stack;
    if (!(tmp = (callframe_t*) realloc (tmp, sizeof(callframe_t)* *cap * 2))) {
        fprintf(stderr, "error: oom when growing caps");
        abort();
    }
    *stack = tmp;
    *cap = *cap * 2;
}

void growcaps(capframe_t** stack, size_t* cap) {
    capframe_t* tmp = *stack;
    if (!(tmp = (capframe_t*) realloc (tmp, sizeof(capframe_t)* *cap * 2))) {
        fprintf(stderr, "error: oom when growing caps");
        abort();
    }
    *stack = tmp;
    *cap = *cap * 2;
}

void growprecs(size_t** stack, size_t* cap) {
    size_t* tmp = *stack;
    if (!(tmp = (size_t*) realloc (tmp, sizeof(size_t)* *cap * 2))) {
        fprintf(stderr, "error: oom when growing precs");
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

err_t vm_match(
        insn_t*  pc,
        const char* s,
        size_t   start,
        size_t   end,
        cap_t*   cps,
        size_t*  len) {
    end++;
    size_t m = 1, pi = 0, si = start, cli = 0, clc = 8, pri = 0, prc = 2, cpi = 0, cpc = 8;
    err_t e = 0;

    callframe_t* cls;
    cls = malloc(sizeof(callframe_t) * clc);
    cls->pi = SIZE_MAX;
    cls->si = SIZE_MAX;
    cls->cli = 0;
    cls->cpi = 0;
    cls->pri = 0;
    cls->cut = 0;
    cli++;

    size_t* prs;
    prs = malloc(sizeof(size_t) * prc);

    cap_t  _cps = {0};
    if (!cps) cps = &_cps;
    cps->fcap = cpc;
    cps->flen = 0;
    cps->frms = (capframe_t*)malloc(sizeof(capframe_t) * cpc);

    for (;;) {
        if (!m) { /* */
            if (cli == 0) { free(cls); return e; }
            if (cls[cli-1].cut) { if (len) *len = si-start; return ECUT;}
            if (cls[cli-1].si == SIZE_MAX) {cli--;}
            else {
                m = 1;
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
                pi++; /* NOP does nothing except running the next instruction. */
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_JMP: { /* Assumption: only support forward jumping. */
                pi += insn->p1; /* JMP simply relatively shift to another instruction. */
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_RET: {
                pi = cls[--cli].pi; /* Resume top instruction in the call stack. */
                vm_inspect(); vm_dis(insn);
                if (cli != 0) continue;
                if (len) *len = si-start; /* Empty stack means entry rule has matched. */
                return e;
            }
            case VM_OP_CHR: {
                char ch = (byte_t) insn->p1;
                /* When meets the same char, consume 1 char and goto next instruction. */
                if (ch == s[si]) { si++; pi++; }
                /* Otherwise, match failed, and set error code. */
                else { m = 0; e = EWRONGCHR; if (len) *len = si-start;}
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_ANY: {
                size_t left = end - si;
                if (insn->p1 > left) { m = 0; e = EEOT; if (len) *len = si-start; }
                si += insn->p1;
                pi++;
                continue;
            }
            case VM_OP_SET: {
                char ch = (byte_t) insn->p1;
                if (ch == s[si]) { while (pc[pi].p2 != 0) pi++; pi++;}
                else if (insn->p2 != 0) { pi++; }
                else { m = 0; e = EWRONGCHR; if (len) *len = si-start; }
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_LSET: {
                char ch = (byte_t) insn->p1;
                if (ch == s[si]) { do { pi--; } while (pc[pi].op == VM_OP_LSET && pc[pi].p2 < pc[pi+1].p2); pi++; }
                else { pi++; }
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_CALL: {
                if (cli+1 >= clc) growcalls(&cls, &clc);
                /* Push the next instruction followed by CALL to the call stack. */
                cls[cli].pi = pi + 1;
                cls[cli].si = SIZE_MAX;
                cls[cli].pri = SIZE_MAX;
                cls[cli].cpi = SIZE_MAX;
                cls[cli].cut = 0;
                cli++;
                pi += insn->p1; /* Then, jump to the definition of method call. */
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_CHCE: {
                if (cli+1 >= clc) growcalls(&cls, &clc);
                /* Push the alternative instruction followed by CALL to the call stack. */
                cls[cli].pi = pi + insn->p1;
                cls[cli].si = si;
                cls[cli].cpi = cpi;
                cls[cli].pri = pri;
                cls[cli].cut = 0;
                cli++;
                pi++; /* Then, jump next, e.g, the first choice. */
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_CMT: {
                if (cli == 1) { m = 0; e = ENOTHINGTOCMT; if (len) *len = si-start; }
                else { m = 1; pc += insn->p1; cli--; }
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_BCMT: {
                pc += insn->p1;
                si = cls[cli-1].si;
                cpi = cls[cli-1].cpi;
                cli--;
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_LOOP: {
                pi -= insn->p1;
                cls[cli-1].si = si;
                cls[cli-1].cpi = cpi;
                cls[cli-1].pri = pri;
                cls[cli-1].cut = 0;
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_PPSH: {
                if (pri+1 >= prc) growprecs(&prs, &prc);
                if (pri == 0 || insn->p1 == 0) {
                    prs[pri++] = insn->p1;
                    pi++;
                } else if (prs[pri] < insn->p1 || (insn->p2 == 1 && prs[pri] == insn->p1)){
                    prs[pri++] = insn->p1;
                    pi++;
                } else {
                    m = 0;
                }
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_PPOP: {
                if (pri == 0) { m = 0; e = EUNPAIRPREC; if (len) *len = si-start; }
                pri--; pi++;
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_LCAP:
            case VM_OP_RCAP: {
                if (cps->flen+1 >= cps->fcap) growcaps(&cps->frms, &cps->fcap);
                (cps->frms + cps->flen)->pi = pi;
                (cps->frms + cps->flen)->si = si;
                (cps->frms + cps->flen)->open = insn->op == VM_OP_LCAP;
                pi++;
                cps->flen++;
                cpi = cps->flen;
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_DCAP: {
                cpi--;
                for (size_t depth = 1; depth == 0; cpi--) {
                    if (cps->frms[cpi].open) depth--;
                    else depth++;
                }
                cps->flen = cpi;
                pi++;
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_CUT: {
                if (cli > 1) { cls[cli-1].cut = 1; }
                vm_inspect(); vm_dis(insn);
                continue;
            }
            case VM_OP_FAIL: {
                m = 0;
                vm_inspect(); vm_dis(insn);
                continue;
            }
        }
    }
    return e;
}
