

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