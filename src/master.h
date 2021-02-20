
#ifndef DEUS_MASTER_HPP
#define DEUS_MASTER_HPP

#ifndef MASTER_MAX_COUNT
#define MASTER_MAX_COUNT 256
#endif

#include <core/deus_list.h>

struct master {
    int id;
    struct deus_list_head head;
};

struct master *master_create(void);

void master_clear_all(void);

int master_get_id(struct master*);

#endif