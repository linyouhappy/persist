#ifndef H_EVENT_INCLUDED
#define H_EVENT_INCLUDED

#include "persist.h"

typedef struct event_s                  event_t;
typedef event_t *                       event_p;
typedef event_p *                       event_pp;

typedef struct event_module_s           event_module_t;
typedef event_module_t                  event_module_p;

typedef struct event_actions_s          event_actions_t;
typedef event_actions_t *               event_actions_p;

typedef void (*event_process_pt)(event_p ev);

//KQUEUE
#include "kqueue.h"

struct event_s {
    int                                 index;
    void                               *data;

    unsigned                            write:1;

    unsigned                            accept:1;

    unsigned                            active:1;

    unsigned                            disabled:1;

    unsigned                            closed:1;

    event_process_pt                    process;

    event_p                             next;
    event_pp                            prev;
};


struct event_actions_s {
    int   (*add)(event_p ev, int event, int flags);
    int   (*del)(event_p ev, int event, int flags);

    int   (*enable)(event_p ev, int event, int flags);
    int   (*disable)(event_p ev, int event, int flags);

    int   (*add_conn)(connection_t *c);
    int   (*del_conn)(connection_t *c, int flags);

    int   (*process_changes)();
    int   (*process_events)();

    int   (*init)();
    void  (*done)();
};

struct event_module_s {
    char *              name;
    event_actions_t     actions;
};

extern event_actions_t   event_actions;

#endif
