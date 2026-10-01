# HTTP server in C

[github repo](https://github.com/yiannis-cunning/Website)

This website is running on a http server writen in c. It utilizes sockets and threads to respond to valid GET/POST http requests.

The basic structure of this server is show below.

```c
int main(){

       // Setup socket on port 80 (http)
       //           socket(), bind(), ...
       while(true){
              // Listen for new connection
              //     listen()
              // Make new thread to parse https requests and respond
              //     pthread_create(), pthread_detach(), read/write()
       }

}
```