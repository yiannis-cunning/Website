#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/un.h>
#include <pthread.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "messages.h"
#include "text_exp.h"

#define max_read 4096
#define max_clients 200
#define WEBSITE_ROOT_LITERAL "/home/yiannis/Desktop/Website/src"


static char *website_root = WEBSITE_ROOT_LITERAL;
static int website_root_len = sizeof(WEBSITE_ROOT_LITERAL) - 1;
static int fd;
static struct sockaddr_in sockaddr = {0};
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int counter = 0;
static int tid_cur = 0;

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
  passert(sig_ret != SIG_ERR, "Error setting up signal handelrs");
}


void end_session(int client_fd, void *arg, char *msg);

