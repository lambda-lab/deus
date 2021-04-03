
#include "master.h"
#include "context.h"

#define NULL 0

static int count = 0;
static int nextId = 0;
static struct master masters[256];

#define DEVICE_NAME_MAX 32

struct master *master_create(void) {
    if (count >= MASTER_MAX_COUNT) { return NULL; }
    char path[DEVICE_NAME_MAX];
    deus_context_get()->kernel_sprintf(path, "deus-master%d", nextId);
    int error = deus_context_get()->kernel_create_device(path);
    if (error) return NULL;
    struct master *mst = &masters[nextId++];
    mst->id = nextId-1;
    count++;
    return mst;
}

int master_get_id(struct master* mst) {
    if (mst == NULL) return -1;
    return mst - masters;
}

void master_clear_all(void) {
    count = 0;
    nextId = 0;
}

int master_delete(struct master *masterToDelete) {
    if (masterToDelete == NULL) return -1;
    if (count > 0) {
        count--;
        if (deus_context_get()->kernel_remove_device(masterToDelete->id) != 0) {
            return -2;
        }
        return 1;
    }
    return -1;
}


int master_count_get
(void) {
    return count;
}