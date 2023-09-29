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


#define max_read 4096;


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


void parse_text_to_html(char *filename){
  f = open(filename, O_RDONLY);
  
}






int main(){

    register_signal(SIGINT);
    register_signal(SIGTERM);

    int client;
    struct sockaddr_in sockaddr = {0};
    struct sockaddr addr_client = {0};
    socklen_t addr_client_len = {0};
    char buffer[4096];
    int n;



    // Make the socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    passert(fd != -1, "Error getting socket!");

    // ##### For debugging - clears the cache of the server from the OS right away??
    int tr = 1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int));

    // Bind to the port and ip of computer
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(80);  //443 for https
    inet_aton("192.168.4.47", (struct in_addr *)&sockaddr.sin_addr.s_addr);
    passert(bind(fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in)) != -1, "Error trying to bind!");

    // Set up with kernel
    passert(listen(fd, 0) != -1, "Error setting up queue in kernel!");




    int home_fd = open("Home.html", O_RDONLY);
    char *hdr = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nConnection: Keep-Alive\n\n";
    int hdr_len = strlen(hdr);

    // Accept loop
    while(1){
        client = accept(fd, &addr_client, &addr_client_len);
        if(client == -1){
            printf("Error accepting client!");
            continue;
        }

        // Read what they have to say
        n = read(client, buffer, 4096);

        // Write the header first
        write(client, hdr, hdr_len);
        
        // Write the rest
        lseek(home_fd, 0, SEEK_SET);
        n = read(home_fd, buffer, 4096);
        while(n > 0){
            write(client, buffer, n);
            n = read(home_fd, buffer, 4096);
        }
        
        
        close(client);

    }



    char *response = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nConnection: Keep-Alive\n\n<!DOCTYPE html>\n<head>\n    Fuck you. \n</head>\n\n</html>\n";

}

//Transfer-Encoding: chunked