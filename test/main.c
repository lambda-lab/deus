#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "tests.h"

int main(void) {
    CU_initialize_registry();
    
    CU_pSuite nothing_suite = CU_add_suite_with_setup_and_teardown(
        "nothing", 
        NULL, 
        NULL, 
        nothing_setup, 
        nothing_teardown
    );
    CU_add_test(nothing_suite, "test_nothing", test_nothing);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0; 
}