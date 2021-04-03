
#include <CUnit/CUnit.h>
#include "tests.h"

#include <master.h>
#include "context.h"
#include <string.h>
#include <stdio.h>

static int create_device_calls = 0;
static int create_device_call_result = 0;
char create_device_arg1[100] = "\0";

static int remove_device_calls = 0;
static int remove_device_call_result = 0;
int remove_device_arg1 = -1;


int create_device_dummy(char* devicename) {
    create_device_calls ++;
    if (create_device_arg1[0] != '\0') {
        CU_ASSERT_STRING_EQUAL(devicename, create_device_arg1);
    }
    return create_device_call_result;
}

int remove_device_dummy(int index) {
    remove_device_calls++;
    if (remove_device_arg1 != -1) {
        CU_ASSERT_EQUAL(index, remove_device_arg1);
    }
    return remove_device_call_result;
}

struct deus_context my_context = {
    .kernel_create_device = create_device_dummy,
    .kernel_remove_device = remove_device_dummy,
    .kernel_sprintf = sprintf,
};

void master_test_setup(void) {
    deus_context_set(&my_context);
    master_clear_all();
    strcpy(create_device_arg1, "\0");
    remove_device_arg1 = -1;
}

void master_test_teardown(void) {
}

REGISTER_TEST_SUITE(master, master_test_setup, master_test_teardown);


void test_create_master(void) {
    master_clear_all();
    struct master *mst = master_create();
    CU_ASSERT_NOT_EQUAL(mst, NULL);
}



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

void test_create_master_creates_device_correct_path(void) {
    create_device_calls = 0;
    strcpy(create_device_arg1, "deus-master0");
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



void test_delete_on_empty(void) {
    int result = master_delete(NULL); 
    CU_ASSERT_EQUAL(result, -1);
}

void test_delete_created_master(void) {
    struct master *mst1 = master_create();
    CU_ASSERT_EQUAL(master_delete(mst1), 1);
}


void test_delete_created_master_count_should_be_zero(void) {
    struct master *mst1 = master_create();
    master_delete(mst1);
    CU_ASSERT_EQUAL(master_count_get(), 0);
}

void test_two_masters_delete_last_id_should_be_one(void) {
    struct master *mst1 = master_create();
    struct master *mst2 = master_create();
    master_delete(mst2);
    struct master *newMaster = master_create();
    CU_ASSERT_EQUAL(master_get_id(newMaster), 2);
}

void test_master_delete_null(void) {
    struct master* mst = master_create();
    CU_ASSERT_EQUAL(master_delete(NULL), -1);
}

void test_delete_first(void) {
    struct master *m1 = master_create();
    struct master *m2 = master_create();
    int result = master_delete(m1);
    CU_ASSERT_EQUAL(result, 1);
    struct master *m3 = master_create();
    CU_ASSERT_EQUAL(master_get_id(m3), 2);
    CU_ASSERT_EQUAL(master_count_get(), 2)
}

void test_master_delete_calls_remove_device(void) {
    remove_device_calls = 0;
    remove_device_call_result = 0; // success 
    struct master *m = master_create();
    master_delete(m);
    CU_ASSERT_EQUAL(remove_device_calls, 1);
}


void test_master_delete_calls_remove_device_error(void) {
    remove_device_calls = 0;
    remove_device_call_result = 1;
    struct master *m = master_create();
    int result = master_delete(m);
    CU_ASSERT_EQUAL(remove_device_calls, 1);
    CU_ASSERT_EQUAL(result, -2);
}

void test_master_delete_calls_remove_device_with_proper_index(void) {
    struct master *m = master_create();
    m = master_create();
    m = master_create();
    m = master_create();
    m = master_create();
    remove_device_arg1 = 5;
    remove_device_calls = 0;
    remove_device_call_result = 0; // success
    m = master_create();
    int result = master_delete(m);
    CU_ASSERT_EQUAL(remove_device_calls, 1);
    CU_ASSERT_EQUAL(result, 1);
}

REGISTER_TEST_S(master, test_create_master)
REGISTER_TEST_S(master, test_create_master_has_id);
REGISTER_TEST_S(master, test_create_master_more_instances)
REGISTER_TEST_S(master, test_create_master_more_instances_until_not_allocated)
REGISTER_TEST_S(master, test_create_master_creates_device)
REGISTER_TEST_S(master, test_create_master_creates_device_correct_path)
REGISTER_TEST_S(master, test_create_master_creates_device_error)
REGISTER_TEST_S(master, test_delete_on_empty)
REGISTER_TEST_S(master, test_delete_created_master)
REGISTER_TEST_S(master, test_delete_created_master_count_should_be_zero)
REGISTER_TEST_S(master, test_two_masters_delete_last_id_should_be_one)
REGISTER_TEST_S(master, test_master_delete_null)
REGISTER_TEST_S(master, test_delete_first)
REGISTER_TEST_S(master, test_master_delete_calls_remove_device)
REGISTER_TEST_S(master, test_master_delete_calls_remove_device_error)
REGISTER_TEST_S(master, test_master_delete_calls_remove_device_with_proper_index)