#pragma once

#include "types.h"

void vm_dis(insn_t* pc);

err_t vm_match(
        insn_t*  pc,
        const char* subject,
        size_t   start,
        size_t   end,
        cap_t*   cap,
        size_t*  len);
