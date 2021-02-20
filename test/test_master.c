
#include <CUnit/CUnit.h>
#include "tests.h"

#include <master.h>
#include "context.h"

static int create_device_calls = 0;
static int create_device_call_result = 0;

int create_device_dummy(char* devicename) {
    create_device_calls ++;
    return create_device_call_result;
}


struct deus_context my_context = {
    .kernel_create_device = create_device_dummy,
};
void test_create_master(void) {
    master_clear_all();
    struct master *mst = master_create();
    CU_ASSERT_NOT_EQUAL(mst, NULL);
}



void master_test_setup(void) {
    deus_context_set(&my_context);
    master_clear_all();
}

void master_test_teardown(void) {
}

REGISTER_TEST_SUITE(master, master_test_setup, master_test_teardown);


void test_create_master_has_id(void) {
    struct master *mst = master_create();
    CU_ASSERT_EQUAL(master_get_id(mst), 0);
}


void test_create_master_more_instances(void) {
    struct master *mst1 = master_create();
    struct master *mst2 = master_create();
    CU_ASSERT_EQUAL(master_get_id(mst1), 0);
    CU_ASSERT_EQUAL(master_get_id(mst2), 1);
}
void test_create_master_more_instances_until_not_allocated(void) {
    int i;
    for (i=0; i<MASTER_MAX_COUNT; i++) {
        struct master *mst1 = master_create();
        CU_ASSERT_NOT_EQUAL(mst1, NULL);
    }
    struct master *mst2 = master_create();
    CU_ASSERT_TRUE(master_get_id(mst2) < 0);
    CU_ASSERT_EQUAL(mst2, NULL);
}


void test_create_master_creates_device(void) {
    create_device_calls = 0;
    struct master *mst1 = master_create();
    CU_ASSERT_EQUAL(create_device_calls, 1);
}

void test_create_master_creates_device_error(void) {
    create_device_call_result = 1;
    create_device_calls = 0;
    struct master *mst1 = master_create();
    CU_ASSERT_EQUAL(create_device_calls, 1);
    CU_ASSERT_EQUAL(mst1, NULL);
    create_device_call_result = 0;
    struct master *mst2 = master_create();
    CU_ASSERT_EQUAL(master_get_id(mst2), 0);
}

REGISTER_TEST_S(master, test_create_master)
REGISTER_TEST_S(master, test_create_master_has_id);
REGISTER_TEST_S(master, test_create_master_more_instances)
REGISTER_TEST_S(master, test_create_master_more_instances_until_not_allocated)
REGISTER_TEST_S(master, test_create_master_creates_device)
REGISTER_TEST_S(master, test_create_master_creates_device_error)