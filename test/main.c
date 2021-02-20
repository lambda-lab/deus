#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "tests.h"

CU_pSuite SUITE_NAME(dflt);

void __attribute__(( __constructor__ (105) )) __setup_tests(void) {
    CU_initialize_registry();
    SUITE_NAME(dflt) = CU_add_suite_with_setup_and_teardown(
        "default", 
        NULL, 
        NULL, 
        nothing_setup, 
        nothing_teardown
    );
}

int main(void) {
    // CU_initialize_registry();
    
    // CU_pSuite nothing_suite = CU_add_suite_with_setup_and_teardown(
    //     "nothing", 
    //     NULL, 
    //     NULL, 
    //     nothing_setup, 
    //     nothing_teardown
    // );
    // CU_add_test(nothing_suite, "test_nothing", test_nothing);
    // CU_add_test(nothing_suite, "test_master_register_simple", test_master_register_simple);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0; 
}