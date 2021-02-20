
#include <CUnit/CUnit.h>
#include "tests.h"


void nothing_setup(void) {

}

void nothing_teardown(void) {
    
}

void test_nothing(void) {
    CU_PASS("Passed!")  
}

REGISTER_TEST (test_nothing )