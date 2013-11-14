#ifndef H_NETWORK_EVENT_INCLUDED
#define H_NETWORK_EVENT_INCLUDED

#include "network/network.h"

typedef struct event_s                  event_t;
typedef event_t *                       event_p;
typedef event_p *                       event_pp;

typedef void (*event_process_pt)(event_p ev);

typedef struct event_module_s           event_module_t;
typedef event_module_t                  event_module_p;

typedef struct event_actions_s          event_actions_t;
typedef event_actions_t *               event_actions_p;


// KQUEUE
#define NGX_READ_EVENT     EVFILT_READ
#define NGX_WRITE_EVENT    EVFILT_WRITE
#define NGX_VNODE_EVENT    EVFILT_VNODE
#define NGX_CLOSE_EVENT    EV_EOF
#define NGX_LOWAT_EVENT    EV_FLAG1
#define NGX_FLUSH_EVENT    EV_ERROR
#define NGX_ONESHOT_EVENT  EV_ONESHOT
#define NGX_CLEAR_EVENT    EV_CLEAR
#define NGX_DISABLE_EVENT  EV_DISABLE

//KQUEUE
#include "network/event/kqueue.h"

struct event_s {
    int                                 index;
    void                               *data;

    unsigned                            write:1;

    unsigned                            accept:1;

    unsigned                            active:1;

    unsigned                            disabled:1;

    unsigned                            closed:1;

    event_process_pt                    process;

//    event_p                             next;
//    event_pp                            prev;
};
//
//
struct event_actions_s {
    int   (*add)(event_p ev, int event, int flags);
    int   (*del)(event_p ev, int event, int flags);

    int   (*enable)(event_p ev, int event, int flags);
    int   (*disable)(event_p ev, int event, int flags);

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

int event_init();
//
#endif
