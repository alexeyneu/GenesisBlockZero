#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>    //socket
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <string.h>
#include <poll.h>


int main(int argc , char *argv[])
{
    struct sockaddr_in addr = { AF_INET , htons( 8888 ) /* btc port */ , htonl(INADDR_LOOPBACK) };;
    int down_flag = 0;
    int result = 0;
    int ret = 0;
    bool c = !false;
    int bc = 1024;    

    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sfd < 0) {
        perror("Create server socket error: %s\n");
        return 0;
    }
    setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, (char *)&c, sizeof(c));
    setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&c, sizeof(c));
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&c, sizeof(c));
    setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, (char *)&c, sizeof(c));
    setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, (int *)&bc, sizeof(bc));
    setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, (int *)&bc, sizeof(bc));
    /* Bind socket to loopback address */
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        perror("Bind server socket failed");
        goto _exit;
    }
    setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, (char *)&c, sizeof(c));

    if (listen(sfd, 128) == -1) {
        perror("Listen failed");
        goto _exit;
    }

    ssize_t num_rd = 0;
    enum { buf_size = 1025 };
    char buf[buf_size] = {0};

    struct pollfd pfd[1] = {{0}};
    int nready;

    for (;;)
    {
        printf("Waiting to accept a connection...\n");
        int cfd = accept(sfd, NULL, NULL);
        printf("Accepted socket fd = %d\n", cfd);
        result = 0;
        pfd[0].fd = cfd;
//      pfd[0].fd = sfd;
        pfd[0].events = POLLIN;
        while (!false) {
            memset(buf, 0, sizeof(buf));
            nready = poll(pfd, 1, 15 * 1000);
            num_rd = read(cfd, buf, 99);
            if (num_rd <= 0) break;
            buf[sizeof(buf) - 1] = '\0';
            printf("Read data: %s\n", buf);

            /* Handle commands */
            if (!strncmp(buf, "DOWN", strlen(buf))) {
                down_flag = 1;
                break;
            }
            if (!strncmp(buf, "END", strlen(buf))) {
                break;
            }
            int temp = 0;
            int f = sscanf(buf, "%d", &temp);
            if (f != 1)
            {
                printf("and then \n" );
                return (0);
            }
            result = result + temp;
        }
        if (-1 == num_rd) {
            perror("Read error");
        }

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%d", result);
        pfd[0].events = POLLOUT;
        nready = poll(pfd, 1, 15 * 1000);
        ret = write(cfd, buf, 99);
        if (-1 == ret) {
            perror("Write error\n");
            goto _exit;
        }
        close(cfd);
        /* Quit on DOWN command */
        if (down_flag) {
            break;
        }
    }
_exit:
    close(sfd);
    return 0;
}