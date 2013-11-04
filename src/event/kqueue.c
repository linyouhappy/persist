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

//    event_actions = event_module_kqueue.actions;

    nchanges = 0;
    return success;
}

int
kqueue_process_events(){
    int          ret, i;
    event_p      ev, re, wr;
    kevent_p     ke;
    connection_p cn;

//    EV_SET(&changes, 3, EVFILT_READ, EV_ADD, 0, 0, NULL);
//    ret = kevent(kq, changes, 1, NULL, 0, NULL);

    for(;;) {
        printf("wait\n");
        ret= kevent(kq, null, 0, events, nevents, NULL);
        if (-1 == ret) {
            printf("kevent() failed!%s\n", strerror(errno));
            exit(0);
        }
        int client;

        printf("count:%d\n", ret);
        for(i=0; i<ret; i++) {
            ke = (kevent_p) &events[i];
            ev = (event_p)  ke->udata;
            cn = (connection_p) ev->data;

            switch(ke->filter) {
            case EVFILT_READ:
                printf("%d:read\n", cn->fd);
                client = accept(cn->fd, NULL, NULL);

                printf("client:%d\n", client);
                printf("%s\n", "success");
//                re = (event_p) cn->read;
//                if (re->process) {
//                    re->process(ev);
//                }
                break;
            case EVFILT_WRITE:
                printf("%d:write\n", cn->fd);
                exit(1);
//                wr = (event_p) cn->write;
//                if (wr->process) {
//                    wr->process(ev);
//                }
                break;
            }
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

    ret = kqueue_ev_set(ev, event, EV_DELETE | EV_DISABLE | flags);
    return ret;
}

int
kqueue_ev_set(event_t *ev, int filter, int flags) {
    connection_p   c;
    kevent_p       ke;
    c = ev->data;

    ke = changes + (nchanges-1);
    ke->ident = c->fd;
    ke->filter = filter;
    ke->flags  = flags;
    ke->fflags = 0;
    ke->data   = 0;
    ke->udata  = ev;

    if (-1 == (kevent(kq, changes, nchanges, NULL, 0, NULL))) {
        printf("kevent() failed!(%s)\n", strerror(errno));
        exit(1);
        return failed;
    }

    return success;
}
