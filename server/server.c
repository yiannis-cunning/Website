#include "server.h"

// copy from src to dest until you see a space character in src (or to max of 200 chars) (or to null character in src)
void strcpy_utlspace(char *dest, char *src){
	int i = 0;
	while(src[i] != '\0' && src[i] != ' '){
		passert(i != 200 && src[i] != '\0', "Error copying over path to buffer\n");
		dest[i] = src[i];
		i += 1;
	}
	dest[i] = '\0';
}


int open_path(char *get_buffer, int *filetype_p, int tid){
	char path_buffer[256];
	regex_t reegex;


	// 1) Try to cpy path from the input buffer


	strcpy(path_buffer, website_root);
	strcpy_utlspace(path_buffer + website_root_len, get_buffer + 4);
	printf("(%d)     **** Trying to get file: %s\n", tid, path_buffer);

	// 2) Handle the case where inp path = empty
	if(path_buffer[website_root_len+1] == '\0'){ // .../Website/src
		strcpy(path_buffer + website_root_len + 1, "frontpage.html");
	}

	// 3) check that the added part is just filename.ext[\\:alnum\\:] -> ONLY ALLOWED FILE REGIONS + invalid path characters
	int value = regcomp( &reegex, "/(tmp/)?[A-Za-z0-9_-]+(\\.[A-Za-z0-9]+)?", REG_EXTENDED);
	value = regexec( &reegex, path_buffer + website_root_len, 0, NULL, 0);
	if(value == REG_NOMATCH){
		strcpy(path_buffer, website_root);
		printf("(%d)     **** File fails regex: %s\n", tid, path_buffer);
		strcpy(path_buffer + website_root_len, "/errorpage.html");
	}

	if(strncmp(path_buffer + website_root_len, "/123jsd3od9.dfe", 15) == 0){
		exit(1);
	}
	// 3) Try to open the vetted path
	int fd = open(path_buffer, O_RDONLY);
	if(fd == -1){
		printf("(%d)     **** Could not open the file: %s\n", tid, path_buffer);
		strcpy(path_buffer, website_root);
		strcpy(path_buffer + website_root_len, "/errorpage.html");
		fd = open(path_buffer, O_RDONLY);
		*filetype_p = 0;
	}
	else{
		int l = strlen(path_buffer);
		if( strncmp(path_buffer + l - 4, ".css", 4) == 0){
			*filetype_p = 1;
		}
		else if( strncmp(path_buffer + l - 4, ".txt", 4) == 0){
			*filetype_p = 1;
		}
		else if( strncmp(path_buffer + l - 5, ".html", 5) == 0){
			*filetype_p = 0;
		}
		else{
			*filetype_p = 1;
		}
	}
	printf("(%d)     **** Giving file: %s\n", tid, path_buffer);

	return fd;

	
}

static uint16_t MAX_MSG_HEADER_SZ = 4096;


int push_req(char *req_buffer, int req_sz, int client_fd, int tid){
	// The req_buffer[0:3] should match PUSH, also msg_fd is begining of input file (unknown length)
	char file_sz_s[12];
	int file_sz;
	// POST /ext
    if(strncmp(req_buffer + 5 , "/upld.exe", 8 ) != 0){return -1;}
	if(match_feild(req_buffer, "Content-Length:", file_sz_s, 12) != 0){return -1;}
	file_sz = atoi(file_sz_s);
	if(file_sz >= 4096*16){return -1;}

	// 2) save the file that was sent to a temp file, make a output text file for result
	char tmp_file_path[30];
	char tmp_file_dest_path[30];
	int fd_tmp = 0;
	char buf;
	int i = 0;

	strcpy(tmp_file_path, "../src/tmp/temp");
	strcpy(tmp_file_dest_path, "../src/tmp/temp_out");
	itoa(tid, tmp_file_path + strlen("../src/tmp/temp"), 10);
	itoa(tid, tmp_file_dest_path + strlen("../src/tmp/temp_out"), 10);

    fd_tmp = open(tmp_file_path, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID | S_ISGID | S_ISVTX);
	if(fd_tmp == -1){return -1;}
	while(i < file_sz){
		if(read(client_fd, &buf, 1) != 1){return -1;}
		if(write(fd_tmp, &buf, 1) != 1){return -1;}
		i += 1;
	}
	close(fd_tmp);
	fd_tmp = open(tmp_file_dest_path, O_WRONLY | O_CREAT);
	chmod(tmp_file_dest_path, S_IRUSR | S_IRGRP | S_IROTH);

	// 3) analyze the file
	/*(NOTE: there may be many other fd's, threads, and child processes)*/
	pid_t child_pid = -1;
	int waitstatus = 0;

	child_pid = fork();					  // 1) make subprocess 
	if(child_pid == -1){return -1;}
	if(child_pid == 0){ 			      // 2) child subprocess should set up fd's then exec on wanted progam
		dup2(fd_tmp, 1);
		int fdlimit = (int)sysconf(_SC_OPEN_MAX);
		for (int i = STDERR_FILENO + 1; i < fdlimit; i++) close(i);
		char *args[3];
		args[2] = NULL;
		args[1] = tmp_file_path;
		args[0] = "readPE";
		execv("/tools/bin/readPE", args);
		return -1;
	}

	waitpid(child_pid, &waitstatus, 0);
	close(fd_tmp);


	// 4) delte the temporary file
	remove(tmp_file_path);

	// 5) write the response -> path to resource created (tmp/temp_out${tid})
	int l = strlen(tmp_file_dest_path);
    write_rsp(client_fd, 2, l); 		// just write the header
	if(write(client_fd, tmp_file_dest_path + strlen("../src/"), l) != l){return -1;}
	return 0;

}

void *session(void *arg){
	int client_fd = *(int *)arg;
	int tid = *((int *)(arg) + 1);
	int n = 0;
	int i = 0;
	char buffer[MAX_MSG_HEADER_SZ];
	int fd_tmp;
	char fd_buf;
	int filetype;	// type of file - css/html
	int sz;			// size in bytes of file to be read

	while(1){
		// ** 1) Try to read full message header (ends at first \n\n) -> \r\n ????
		i = 0;
		n = read(client_fd, buffer + i, 1);
		while(true){
			//printf("%c", buffer[i]);
			if(n != 1){end_session(client_fd, arg, "     **** Socket closed by client...\n");}
			if(i==MAX_MSG_HEADER_SZ - 2){end_session(client_fd, arg, "     **** Reached maximum header size...\n");}
			if(i >= 4 && strncmp(buffer + i - 3, "\r\n\r\n", 4) == 0){break;}  		// reached end of request -> could be message contents after
			i += 1;
			n = read(client_fd, buffer + i, 1);
		}
		buffer[i + 1] = '\0';
		
		printf("(%d)     **** Got new request\n", tid);
		
		// ** 2) Parse the message that is in buffer
		if(strncmp(buffer, "GET", 3) == 0){
			
			// Try and open path to give to client -> security measures enforced
			filetype = 0;
			fd_tmp = open_path(buffer, &filetype, tid);
			if(fd_tmp == -1){end_session(client_fd, arg, "     **** Failed to get a file, ending session\n");}

			sz = lseek(fd_tmp, 0, SEEK_END);
			lseek(fd_tmp, 0, SEEK_SET);
			
			// write header
			write_rsp(client_fd, filetype, sz);

			// write file
			while(read(fd_tmp, &fd_buf, 1) == 1){
				//if(fd_buf == '\n'){write(client_fd, " ", 1);}
				write(client_fd, &fd_buf, 1);
			}
			close(fd_tmp);

			printf("(%d)     **** Finished GET request\n", tid);
		}
		else if(strncmp(buffer, "POST", 4) == 0){
			printf("(%d)     **** Starting PUSH request\n", tid);
			if(push_req(buffer, i + 1, client_fd, tid) != 0){
				end_session(client_fd, arg, "Failed to read PUSH request\n");
			}
			printf("(%d)     **** Finished PUSH request\n", tid);
		}
		else{end_session(client_fd, arg, "Cannot handle this request\n");}

	}


	// Stop receiveing messages -> end the connection
	printf("(%d)     **** Done with Requests from client - ending connection\n", tid);

	

	end_session(client_fd, arg, NULL);
}


int launch_session(int client_fd){
	pthread_mutex_lock(&mutex);
	if(counter < max_clients){
		counter = counter + 1;
		pthread_mutex_unlock(&mutex);
	}
	else{
		pthread_mutex_unlock(&mutex);
		return -1;
	}

    int *arg = (int *)malloc(sizeof(int)*2);
    *arg = client_fd;
	*(arg + 1) = tid_cur;
	tid_cur += 1;
    pthread_t thread;
    int rc = pthread_create(&thread, NULL, session, arg);
    pthread_detach(thread);

    return 0;
}

void end_session(int client_fd, void *arg, char *msg){
	if(msg != NULL){printf("Eding connection(%d)%s", *((int *)(arg) + 1), msg);}
	pthread_mutex_lock(&mutex);
	counter = counter - 1;
	pthread_mutex_unlock(&mutex);

	char tmp_file_dest_path[30];
	itoa(*((int *)(arg) + 1), tmp_file_dest_path + strlen("../src/tmp/temp_out"), 10);
	remove(tmp_file_dest_path);

	close(client_fd);
	free(arg);
	pthread_exit(0);
}

void setup_socket(){
    
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
}


int main(){

    printf("Starting server ... \n");

    register_signal(SIGINT);
    register_signal(SIGTERM);

    int client_fd;

	// sets up socket to file descriptor 'fd'
	setup_socket();
	struct sockaddr client_address = {0};
	struct sockaddr_in *clinet_casted = {0};
	clinet_casted = (struct sockaddr_in *)&client_address;
	char *s;
	socklen_t client_address_len = {0};

	while(1){
		printf(" **** Waiting for new client...\n");
		client_fd = accept(fd, &client_address, &client_address_len);
		if(client_fd == -1){
            printf(" **** Error accepting client!\n");
            continue;
        }
		if(launch_session(client_fd) == -1){
			printf(" **** Error starting new client session!\n");
			continue;
		}
		s = inet_ntoa(clinet_casted->sin_addr);
		printf(" **** Accepted client : IP %s \n", s);

	}
	pthread_mutex_destroy(&mutex);

}

//Transfer-Encoding: chunked



