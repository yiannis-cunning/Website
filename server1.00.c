#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>



static int fd;


void passert(bool cond, char *str){
    if(cond == false){
        int err = errno;
        perror(str);
        exit(err);
    }
}

void close_socket() {
  if (close(fd)) {
    passert(false, "Exited");
  }
  exit(0);
}

void handle_signal(int signum) {
  close_socket();
}


void register_signal(int signum) {
  void (*sig_ret)(int) = signal(signum, handle_signal);
  if (sig_ret == SIG_ERR) {
    int err = errno;
    perror("signal");
    exit(err);
  }
}



int main(){

    register_signal(SIGINT);
    register_signal(SIGTERM);

    int ret;
    int acc;
    struct sockaddr_in sockaddr = {0};
    struct sockaddr addr_client = {0};
    socklen_t addr_client_len = {0};
    char buffer[4096];



    // Make the socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    passert(fd != -1, "Error getting socket");

    // ##### For debugging
    int tr = 1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int));

    // Bind to the port and ip of computer
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(80);  //443 for https
    inet_aton("192.168.4.47", (struct in_addr *)&sockaddr.sin_addr.s_addr);
    ret = bind(fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
    passert(ret != -1, "Error trying to bind");

    // Set up with kernel
    ret = listen(fd, 0);
    passert(ret != -1, "Error setting up queue in kernel");

    // Accept loop
    acc = accept(fd, &addr_client, &addr_client_len);
    passert(acc != -1, "Error accepting connection");

    memset(buffer, 97, 4096);
    int n = read(acc, buffer, 4096);
    write(1, buffer, n);


    char *response = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nConnection: Keep-Alive\n\n<!DOCTYPE html>\n<head>\n    Fuck you. \n</head>\n\n</html>\n";
    n = write(acc, response, strlen(response));


    shutdown(fd, SHUT_RD);
    close(acc);
    close(fd);
}

//Transfer-Encoding: chunked