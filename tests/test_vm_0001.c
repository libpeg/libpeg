#include "tinytest.h"
#include "vm.h"
#include "opcode.h"

void test_char_match(void) {
    printf("---\ntest_char_match\n");
    insn_t pc[] = {{VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}};
    size_t len = 0;
    err_t err = vm_match(pc, "0", 0, 1, 0, &len);
    ASSERT_EQUALS(0, err);
    ASSERT_EQUALS(1, len);
}

void test_char_not_match(void) {
    printf("---\ntest_char_not_match\n");
    insn_t pc[] = {{VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}};
    size_t len = 0;
    size_t err = vm_match(pc, "1", 0, 1, 0, &len);
    ASSERT_EQUALS(0, len);
    ASSERT_EQUALS(EWRONGCHR, err);
}

void test_char_stop_at_where_failed(void) {
    printf("---\ntest_char_stop_at_where_failed\n");
    insn_t pc[] = {{VM_OP_CHR, '0', 0}, {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}};
    size_t len = 0;
    err_t err = vm_match(pc, "01", 0, 1, 0, &len);
    ASSERT_EQUALS(1, len);
    ASSERT_EQUALS(EWRONGCHR, err);
}

int main() {
    RUN(test_char_match);
    RUN(test_char_not_match);
    RUN(test_char_stop_at_where_failed);
    return TEST_REPORT();
}
