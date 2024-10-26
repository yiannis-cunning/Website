# HTTP server in C

[Github repo](https://github.com/yiannis-cunning/Website)

This website is running on a http server writen in c. It utilizes sockets and threads to respond to valid GET/POST http requests.

The basic structure of the server looks like the below.

```c
int main(){

       // Setup socket on port 80 (http)
       while(true){
              // Listen for new connection
              //     Make new thread to Parse https requests and respond
       }

}
```