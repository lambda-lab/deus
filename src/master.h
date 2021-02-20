
#ifndef DEUS_MASTER_HPP
#define DEUS_MASTER_HPP

#ifndef MASTER_MAX_COUNT
#define MASTER_MAX_COUNT 256
#endif


struct master {
    int a;
};



struct master *master_create(void);

void master_clear_all(void);

int master_get_id(struct master*);

#endif 