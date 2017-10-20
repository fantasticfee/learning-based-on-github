typedef struct {
  pthread_t             thread_tid;             /* thread ID */
  long                  thread_count;   /* # connections handled */
  int conn_fd;
  int loc;
} Thread;
Thread  *tptr;          /* array of Thread structures; calloc'ed */

int                             listenfd, nthreads;
socklen_t               addrlen;
pthread_mutex_t mlock;
