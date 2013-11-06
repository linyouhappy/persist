#ifndef H_KQUEUE_INCLUDED
#define H_KQUEUE_INCLUDED

#include "event.h"

 #include <sys/types.h>
 #include <sys/event.h>
 #include <sys/time.h>

/*
 *
struct kevent {
    uintptr_t   ident;      事件ID
    int16_t     filter;     过滤事件: EVFILT_READ, EVFILT_WRIT
    uint16_t    flags;      :EV_ADD, EV_DELETE, EV_ENABLE, EV_DISABLE
    uint32_t    fflags;
    intptr_t    data;
    void        *udata;     自定义数据指针
};
 */
typedef struct kevent           kevent_t;
typedef kevent_t *              kevent_p;

void server_tcp_accept(event_p ev);

int kqueue_init();
int kqueue_add_event(event_p ev, int event, int flags);
int kqueue_del_event(event_p ev, int event, int flags);
int kqueue_ev_set(event_t *ev, int filter, int flags);
int kqueue_process_events();
#endif
