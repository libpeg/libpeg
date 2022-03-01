#include "tinytest.h"
#include "vm.h"
#include "opcode.h"

void test_char_match(void) {
    insn_t pc[] = {{VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}};
    size_t len = 0;
    err_t err = vm_match(pc, "0", 0, 1, 0, &len);
    ASSERT_INT_EQUALS(EOK, err);
    ASSERT_SIZE_EQUALS(1, len);
}

void test_char_not_match(void) {
    insn_t pc[] = {{VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}};
    size_t len = 0;
    size_t err = vm_match(pc, "1", 0, 1, 0, &len);
    ASSERT_INT_EQUALS(EWRONGCHR, err);
    ASSERT_SIZE_EQUALS(0, len);
}

void test_multi_chars_match(void) {
    insn_t pc[] = {{VM_OP_CHR, '0', 0}, {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}};
    size_t len = 0;
    err_t err = vm_match(pc, "00", 0, 1, 0, &len);
    ASSERT_INT_EQUALS(EOK, err);
    ASSERT_SIZE_EQUALS(2, len);
}

void test_multi_chars_match_custom_start_end(void) {
    insn_t pc[] = {{VM_OP_CHR, '0', 0}, {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}};
    size_t len = 0;
    err_t err = vm_match(pc, "100", 1, 3, 0, &len);
    ASSERT_INT_EQUALS(EOK, err);
    ASSERT_SIZE_EQUALS(2, len);
}

void test_char_stop_at_where_failed(void) {
    insn_t pc[] = {{VM_OP_CHR, '0', 0}, {VM_OP_CHR, '0', 0}, {VM_OP_RET, 0, 0}};
    size_t len = 0;
    err_t err = vm_match(pc, "01", 0, 1, 0, &len);
    ASSERT_INT_EQUALS(EWRONGCHR, err);
    ASSERT_SIZE_EQUALS(1, len);
}

int main() {
    RUN(test_char_match);
    RUN(test_char_not_match);
    RUN(test_multi_chars_match);
    RUN(test_multi_chars_match_custom_start_end);
    RUN(test_char_stop_at_where_failed);
    return TEST_REPORT();
}
