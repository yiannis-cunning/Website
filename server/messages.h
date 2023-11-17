#define BASE_HEADER_HTML "HTTP/1.1 200 OK\r\nDate: Sat, 21 Oct 2023 19:23:59 EST\r\nContent-Type: text/html; charset=utf-8\r\nConnection: Keep-Alive\r\n"
#define BASE_HEADER_CSS "HTTP/1.1 200 OK\r\nContent-Type: text/css; charset=utf-8\r\nConnection: Keep-Alive\r\n"
#define BASE_HEADER_POSTOK "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: Keep-Alive\r\n"
#define LENGTH_HEADER "Content-Length: "
#define CRLF "\r\n"

#define BASE_HEADER_HTML_LEN sizeof(BASE_HEADER_HTML) - 1
#define BASE_HEADER_CSS_LEN sizeof(BASE_HEADER_CSS) - 1
#define LENGTH_HEADER_LEN sizeof(LENGTH_HEADER) - 1
#define CRLF_LEN sizeof(CRLF) - 1
#define BASE_HEADER_POSTOK_LEN sizeof(BASE_HEADER_POSTOK) - 1

void itoa(uint32_t n, char *buffer, int base){
    // buffer is at least 11 chars big
    int sz = 0;
    int n2 = n;
    if(n2 == 0){sz = 1;}
    while(n2 != 0){
        n2 = n2/base;
        sz += 1;
    }
    buffer[sz] = '\0';
    sz -= 1;
    while(sz >= 0){
        buffer[sz] = '0' + n%base;
        n = n/10;
        sz -= 1;
    }
}



// Write some header to a fd
void write_rsp(int fd, int base_hdr_type, int content_length){
    // write base header
    switch(base_hdr_type){
        case 0:
            write(fd, BASE_HEADER_HTML, BASE_HEADER_HTML_LEN);
            break;
        case 1:
            write(fd, BASE_HEADER_CSS, BASE_HEADER_CSS_LEN);
            break;
        case 2: // post response header
            write(fd, BASE_HEADER_POSTOK, BASE_HEADER_POSTOK_LEN);
            break;
        default:
            write(fd, BASE_HEADER_HTML, BASE_HEADER_HTML_LEN);
            break;
    }
    if(content_length == 0){
        write(fd, CRLF, CRLF_LEN);
        return;
    }
    // write content-length header
    write(fd, LENGTH_HEADER, LENGTH_HEADER_LEN);
    char buffer[11];
    itoa(content_length, buffer, 10);
    write(fd, buffer, strlen(buffer));
    write(fd, CRLF, CRLF_LEN);

    // write CRLF
    write(fd, CRLF, CRLF_LEN);
    
}