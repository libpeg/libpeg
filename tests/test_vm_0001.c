#include "tinytest.h"
#include "vm.h"
#include "opcode.h"

void test_char_match() {
    printf("---\ntest_char_match\n");
    byte_t pc[] = {VM_OP_CHR, '0', VM_OP_RET};
    size_t len  = vm_match(pc, "0", 0, 1, 0, 0);
    ASSERT_EQUALS(1, len);
}

void test_char_not_match() {
    printf("---\ntest_char_not_match\n");
    byte_t pc[] = {VM_OP_CHR, '0', VM_OP_RET};
    int e = 0;
    size_t len  = vm_match(pc, "1", 0, 1, 0, &e);
    ASSERT_EQUALS(0, len);
    ASSERT_EQUALS(EWRONGCHR, e);
}

int main() {
    RUN(test_char_match);
    RUN(test_char_not_match);
    return TEST_REPORT();
}
