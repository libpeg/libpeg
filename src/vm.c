#include "vm.h"
#include "opcode.h"

#define vm_read() (pc[pi++])
#define vm_next() ({ goto *ptrs[pc[pi++]]; })

void growcaps(callframe_t** stack, size_t* cap) {
    callframe_t* tmp = *stack;
    if (!(tmp = (callframe_t*) realloc (tmp, sizeof(callframe_t)* *cap * 2))) {
        fprintf(stderr, "error: oom when growing caps");
        abort();
    }
    *stack = tmp;
    *cap = *cap * 2;
}

size_t vm_match(
        byte_t*  pc,
        const char* s,
        size_t   start,
        size_t   end,
        cap_t*   cap,
        int*     e) {
    end++;
    cap++;
    size_t m = 1, pi = 0, si = start;
    for (;;) {
        if (!m) {
            return 0;
        }
        byte_t op = pc[pi++];
        switch (op) {
            case VM_OP_CHR: {
                byte_t ch = pc[pi];
                if (ch == s[si]) { si++; pi++; }
                else { m = 0; *e = EWRONGCHR; }
                printf("si=%zu, pi=%zu | CHR %d ('%c')\n", si, pi, ch, ch);
                continue;
            }
            case VM_OP_RET: {
                printf("si=%zu, pi=%zu | RET\n", si, pi);
                return si-start;
            }
        }
    }
    return 0;
}
