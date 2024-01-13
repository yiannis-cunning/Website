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
#include <time.h>


#define max_clients 200
#define WEBSITE_ROOT_LITERAL "/home/yiannis/Desktop/Website/src"
#define WEBSITE_ROOT_LEN_LITERAL sizeof("/home/yiannis/Desktop/Website/src") - 1
#define KILL_PAGE_LITERAL "/123jsd3od93djib.dfe"
#define ALLOWABLE_PAGES_REGEX "/(tmp/)?[A-Za-z0-9_-]+(\\.[A-Za-z0-9]+)?"
#define IP_ADD_LITERAL "192.168.4.47"

static uint16_t MAX_MSG_HEADER_SZ = 4096;
static uint32_t MAX_EXE_INPUT_SZ = 4096*16;
static char *website_root = WEBSITE_ROOT_LITERAL;
static int website_root_len = sizeof(WEBSITE_ROOT_LITERAL) - 1;





static int fd;
static struct sockaddr_in sockaddr = {0};
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int counter = 0;
static uint32_t tid_cur = 0;

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




// expect in form 'feild_name value'\r\n
// Asuming valid req and name 
int match_feild(char *req_buff, char *feild_name, char *buf_out, int buf_sz){
    int i = 0;
    int l = strlen(feild_name);
    while(strncmp(req_buff + i, feild_name, l) != 0 ){
        while(req_buff[i] != '\n'){i+=1;}
        if(req_buff[i-2] == '\n'){return -1;}
        i += 1;
    }
    i = i + l + 1;
    int j = i;
    while(req_buff[j] != '\r'){
        j += 1;
    }
    if(j - i >= buf_sz){return 1;}
    memcpy(buf_out, req_buff + i, j - i);
    buf_out[j - i] = '\0';
    return 0;
}  