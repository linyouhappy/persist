#include "event.h"

event_actions_t   event_actions;

int event_init() {
    return kqueue_init();
}
