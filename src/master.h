
#ifndef DEUS_MASTER_HPP
#define DEUS_MASTER_HPP

#ifndef MASTER_MAX_COUNT
#define MASTER_MAX_COUNT 256
#endif

#include <core/deus_list.h>

struct master {
    struct deus_list_head head;
    int id;
    int deviceIndex;
};

struct master *master_create(void);

void master_clear_all(void);

int master_get_id(struct master*);
int master_delete(struct master *masterToDelete);

int master_count_get(void);

#endif