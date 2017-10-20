#include        "unp.h"
#include        "pthread07.h"

//#define       MAXN    16384           /* max # bytes client can request */

void
web_child(int sockfd, void* loc)
{
//      int                     ntowrite;
        ssize_t         nread;
//      char            line[MAXLINE], result[MAXN];
        char            line[MAXLINE];
        int i;

        //for ( ; ; ) {
                //if ( (nread = Readline(sockfd, line, MAXLINE)) == 0)
again:
                while ((nread = Readline(sockfd, line, MAXLINE)) > 0)
                {
                        for (i = 0; i < nthreads; i++){
                         if(tptr[i].conn_fd != 0)
                                Writen(tptr[i].conn_fd, line, nread);
                        }
                }
                        //return;               /* connection closed by other end */

        if (nread < 0 && errno == EINTR)
                goto again;
        else if (nread < 0){
                tptr[(int) loc].conn_fd=0;
                err_sys("str_echo: read error");
        }
                        /* 4line from client specifies #bytes to write back */
                //ntowrite = atol(line);
                //if ((ntowrite <= 0) || (ntowrite > MAXN))
                //      err_quit("client request for %d bytes", ntowrite);

                //Writen(sockfd, result, ntowrite);
        //}
}
