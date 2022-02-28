#include "tinytest.h"
#include "vm.h"
#include "opcode.h"

void test_call_match() {
    printf("---\ntest_call_match\n");
    insn_t pc[] = {
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}
    };
    size_t len  = vm_match(pc, "0", 0, 1, 0, 0);
    ASSERT_EQUALS(1, len);
}

void test_call_not_match() {
    printf("---\ntest_call_not_match\n");
    insn_t pc[] = {
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}
    };
    int e = 0;
    size_t len  = vm_match(pc, "1", 0, 1, 0, &e);
    ASSERT_EQUALS(len, 0);
    ASSERT_EQUALS(EWRONGCHR, e);
}

void test_nested_call() {
    printf("---\ntest_nested_call\n");
    insn_t pc[] = {
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}
    };
    size_t len  = vm_match(pc, "0", 0, 1, 0, 0);
    ASSERT_EQUALS(1, len);
}

void test_nested_call_not_match() {
    printf("---\ntest_nested_call_not_match\n");
    insn_t pc[] = {
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}
    };
    int e;
    size_t len  = vm_match(pc, "1", 0, 1, 0, &e);
    ASSERT_EQUALS(len, 0);
    ASSERT_EQUALS(EWRONGCHR, e);
}

int main() {
    RUN(test_call_match);
    RUN(test_call_not_match);
    RUN(test_nested_call);
    RUN(test_nested_call_not_match);
    return TEST_REPORT();
}

