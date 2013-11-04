#include "kqueue.h"

static int kq = -1;
static kevent_p  changes;
static kevent_p  events;
static int       nchanges=0, nevents=512;

event_module_t event_module_kqueue = {
     "event_module_kqueue",
     {
         kqueue_add_event,
         kqueue_del_event,
         kqueue_add_event,
         kqueue_del_event,
         NULL,
         NULL,
         NULL,
         NULL,
         kqueue_init,
         null
     }
};

int
kqueue_init() {
    if (-1 == kq) {
        kq = kqueue();

        if (-1 == kq) {
            printf("kqueue() failed!(%s)\n", strerror(errno));
            return failed;
        }
    }

    //  事件列表
    events  = malloc(nevents * sizeof(kevent_t));
    changes = malloc(nevents * sizeof(kevent_t));

    event_actions = event_module_kqueue.actions;

    nchanges = 0;
    return success;
}

int
kqueue_process_events(){
    int ret, i;
    connection_p c;

//    EV_SET(&changes, 3, EVFILT_READ, EV_ADD, 0, 0, NULL);
//    ret = kevent(kq, changes, 1, NULL, 0, NULL);

    for(;;) {
        printf("wait\n");
        ret= kevent(kq, null, 0, events, nevents, NULL);
        printf("%d\n", ret);
        if (-1 == ret) {
            printf("kevent() failed!%s\n", strerror(errno));
            exit(0);
        }

        for(i=0; i<ret; i++) {
            printf("id:%ld\n", events[i].ident);
            c = events[i].data;




//            if (listen->fd == events[i].ident) {
//                // accept
//                for (int i = 0; i < events[i].data; i++) {
//                    int client = accept(listen->fd, NULL, NULL);
//                    printf("client:%d\n", client);
//                    if (client == -1) {
//                        continue;
//                    }
//                    close(client);
//                }
//            } else {
//                // client
//            }
        }
    }
    return success;
}

int
kqueue_add_event(event_p ev, int event, int flags) {
    int ret;

    ev->active   = 1;
    ev->disabled = 0;
    ev->index = nchanges;

    nchanges++;

    ret = kqueue_ev_set(ev, event, EV_ADD | EV_ENABLE | flags);
    return ret;
}

int
kqueue_del_event(event_p ev, int event, int flags) {
    int ret;

    ev->active   = 0;
    ev->disabled = 1;

    ret = kqueue_ev_set(ev, event, EV_DELETE | EV_DELETE | flags);
    return ret;
}

int
kqueue_ev_set(event_t *ev, int filter, int flags) {
    connection_t  *c;
    c = ev->data;

    EV_SET(changes+(nchanges-1), c->fd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    if (-1 == (kevent(kq, changes, nchanges, NULL, 0, NULL))) {
        printf("kevent() failed!(%s)\n", strerror(errno));
        exit(1);
        return failed;
    }

    return success;
}
