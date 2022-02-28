#pragma once

#include "types.h"

size_t vm_match(
        byte_t*  pc,
        const char* subject,
        size_t   start,
        size_t   end,
        cap_t*   cap,
        int*     err);
