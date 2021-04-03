
#ifndef DEUS_CONTEXT_H
#define DEUS_CONTEXT_H

struct deus_context {
    int (*kernel_create_device) (char*);
    int (*kernel_remove_device) (int);
    int (*kernel_sprintf) (char* buf, const char* format, ...);
};

void deus_context_set(const struct deus_context *context);

const struct deus_context *deus_context_get(void);

#endif