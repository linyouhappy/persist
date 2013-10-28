#ifndef H_CONNECTION_INCLUDED
#define H_CONNECTION_INCLUDED

// TODO : 1 connection design
#include "persist.h"

typedef struct connection_s             connection_t;
typedef struct connection_action_s      connection_action_t;

//  conn
struct connection_s {
    connection_action_t   * action;
};

//  conn action
struct connection_action_s {
    void *          read;
    void *          write;
};

#endif
