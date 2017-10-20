#include        "unpthread.h"
#include        "pthread07.h"

void
thread_make(int i)
{
        void    *thread_main(void *);

        Pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *) i);
        return;         /* main thread returns */
}

void *
thread_main(void *arg)
{
        int                             connfd;
        void                    web_child(int,void *);
        socklen_t               clilen;
        struct sockaddr *cliaddr;
        //int loc=*(int*)arg;

        cliaddr = Malloc(addrlen);

        printf("thread %d starting\n", (int) arg);
        //printf("thread %d starting\n", loc);
        for ( ; ; ) {
                clilen = addrlen;
        Pthread_mutex_lock(&mlock);
                connfd = Accept(listenfd, cliaddr, &clilen);
                Pthread_mutex_unlock(&mlock);
                tptr[(int) arg].thread_count++;
                tptr[(int) arg].conn_fd=connfd;

                web_child(connfd, arg);         /* process request */
                Close(connfd);
        }
}
