
#ifndef DEUS_TEST_TESTS_HPP
#define DEUS_TEST_TESTS_HPP

#include <CUnit/CUnit.h>

#define SUITE_NAME(name) __suite_##name

#define REGISTER_TEST_SUITE(name, setup, teardown)                                                    \
    static CU_pSuite SUITE_NAME(name);                                                                \
    void __attribute__((__constructor__(106))) __register_test_##name(void) {                         \
        SUITE_NAME(name) = CU_add_suite_with_setup_and_teardown(#name, 0, 0, (setup), (teardown));    \
    }

#define REGISTER_TEST_S(suite, test)                                                                  \
    void __attribute__((__constructor__( 107 ))) __register_test_##test## with_suite_##suite(void) {  \
        CU_add_test(SUITE_NAME(suite), #test, test);                                                  \
    }

#define REGISTER_TEST(test) REGISTER_TEST_S(dflt, test)

void nothing_setup(void);
void nothing_teardown(void);

extern CU_pSuite SUITE_NAME(dflt);

#endif //  DEUS_TEST_TESTS_HPP