
#include "master.h"
#include "context.h"

#define NULL 0

static int count = 0;
static int nextId = 0;
static struct master masters[256];

struct master *master_create(void) {
    if (count >= MASTER_MAX_COUNT) { return NULL; }
    int error = deus_context_get()->kernel_create_device("");
    if (error) return NULL;
    struct master *mst = &masters[nextId++];
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
        return 1;
    }
    return -1;
}


int master_count_get(void) {
    return count;
}