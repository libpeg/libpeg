#pragma once

#include "types.h"

void vm_dis(insn_t* pc);

size_t vm_match(
        insn_t*  pc,
        const char* subject,
        size_t   start,
        size_t   end,
        cap_t*   cap,
        int*     err);
