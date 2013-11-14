//@TODO 怎么解决 引入 event.h
#include "network/network.h"

event_actions_t   event_actions;

int event_init() {
    return kqueue_init();
}
