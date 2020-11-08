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
    char buf[100] = {0};

    int data_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(data_socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&c, sizeof(c));
    setsockopt(data_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&c, sizeof(c));
   	setsockopt(data_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&c, sizeof(c));
    ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(addr));
    if (-1 == ret) {
        perror("Connect error");
        exit(EXIT_FAILURE);
    }
    setsockopt(data_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&c, sizeof(c));    /* Send arguments */
    
    struct pollfd pfd[1];
	int nready;
	pfd[0].fd = data_socket;
	pfd[0].events = POLLOUT;

   	for (int k = 1; k < argc; k++) 
   	{
   		nready = poll(pfd, 1, 15 * 1000);
       // if((pfd[0].revents & (POLLOUT|POLLHUP))) printf("tray \n" );
        memset(buf, 0, sizeof(buf));
        strcpy(buf, argv[k]);
        ret = write(data_socket,buf , 99);
        if (-1 == ret) {
            perror("Write error");
            break;
        }
    }

    memset(buf, 0, sizeof(buf));
    strcpy(buf, "END");
    nready = poll(pfd, 1, 15 * 1000);
    ret = write(data_socket, buf, 99);
    printf("write %s to socket, ret = %d\n", buf, ret);
    if (-1 == ret) {
        perror("Write to socket error");
        exit(EXIT_FAILURE);
    }
    /* Read the result */
    memset(buf, 0, sizeof(buf));
    pfd[0].events = POLLIN;
    nready = poll(pfd, 1, 15 * 1000);
    ret = read(data_socket, buf, 99);
    if (-1 == ret) {
        perror("Read from client socket error");
        exit(EXIT_FAILURE);
    }
    buf[sizeof(buf) - 1] = '\0';
    printf("Result = %10s\n", buf);
    close(data_socket);
    exit(EXIT_SUCCESS);
}