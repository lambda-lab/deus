
#ifndef DEUS_TEST_TESTS_HPP
#define DEUS_TEST_TESTS_HPP

#include <CUnit/CUnit.h>

extern CU_pSuite default_suite;



#define REGISTER_TEST(name) \
    void __attribute__((__constructor__ ( 106 ))) __register_test_##name(void) { \
        CU_add_test(default_suite, #name, name); \
    }

void nothing_setup(void); 
void nothing_teardown(void);

void test_nothing(void); 




// register master 
void test_master_register_simple(void); 

#endif //  DEUS_TEST_TESTS_HPP