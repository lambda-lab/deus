
#include "context.h"

static const struct deus_context *_context;

void deus_context_set(const struct deus_context *context) {
    _context = context;
}

const struct deus_context *deus_context_get(void) {
    return _context;
}