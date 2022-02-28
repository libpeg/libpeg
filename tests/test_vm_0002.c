#include "tinytest.h"
#include "vm.h"
#include "opcode.h"

void test_call_match() {
    printf("---\ntest_call_match\n");
    insn_t pc[] = {
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}
    };
    size_t len = 0;
    err_t err = vm_match(pc, "0", 0, 1, 0, &len);
    ASSERT_EQUALS(EOK, err);
    ASSERT_EQUALS(1, len);
}

void test_call_not_match() {
    printf("---\ntest_call_not_match\n");
    insn_t pc[] = {
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}
    };
    size_t len = 0;
    err_t err = vm_match(pc, "1", 0, 1, 0, &len);
    ASSERT_EQUALS(EWRONGCHR, err);
    ASSERT_EQUALS(len, 0);
}

void test_nested_call() {
    printf("---\ntest_nested_call\n");
    insn_t pc[] = {
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CALL, 2, 0}, {VM_OP_RET, 0, 0},
        {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}
    };
    size_t len = 0;
    err_t err = vm_match(pc, "0", 0, 1, 0, &len);
    ASSERT_EQUALS(EOK, err);
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
    size_t len = 0;
    err_t e = vm_match(pc, "1", 0, 1, 0, &len);
    ASSERT_EQUALS(0, len);
    ASSERT_EQUALS(EWRONGCHR, e);
}

int main() {
    RUN(test_call_match);
    RUN(test_call_not_match);
    RUN(test_nested_call);
    RUN(test_nested_call_not_match);
    return TEST_REPORT();
}

