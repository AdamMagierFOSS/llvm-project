// RUN: %clang_cc1 -triple x86_64-linux-gnu -emit-llvm -fsanitize=array-bounds,integer-divide-by-zero,shift-exponent -O0 %s -o - | FileCheck %s

// Verify that UBSan checks are not emitted when the check condition
// is constant-folded to true (provably safe).

static int arr[5] = {1, 2, 3, 4, 5};

// CHECK-LABEL: test_const_index
// CHECK-NOT:   __ubsan_handle
// CHECK:       ret i32
int test_const_index(void) {
  return arr[3];
}

// CHECK-LABEL: test_dynamic_index
// CHECK:       __ubsan_handle
// CHECK:       ret i32
int test_dynamic_index(int i) {
  return arr[i];
}

// CHECK-LABEL: test_div_by_const
// CHECK-NOT:   __ubsan_handle
// CHECK:       ret i32
int test_div_by_const(int x) {
  return x / 1;
}

// CHECK-LABEL: test_div_dynamic
// CHECK:       __ubsan_handle
// CHECK:       ret i32
int test_div_dynamic(int x, int y) {
  return x / y;
}

// CHECK-LABEL: test_shift_const_exponent
// CHECK-NOT:   __ubsan_handle
// CHECK:       ret i32
int test_shift_const_exponent(unsigned x) {
  return x >> 3;
}

// CHECK-LABEL: test_shift_dynamic_exponent
// CHECK:       __ubsan_handle
// CHECK:       ret i32
int test_shift_dynamic_exponent(unsigned x, unsigned y) {
  return x >> y;
}
