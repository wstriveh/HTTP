#include <stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>   
#include<arpa/inet.h>

int main(int argc,char* argv[])
{
    if(3 != argc)
    {
        printf("usage: ./server[ip] [port]\n");
        return (1);
    }
    
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)

    {
        perror("socket");
        return 1;    
    }

    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
   // addr.sin_addrs.s_addr = inet_addr(argv[1]);
    inet_pton(AF_INET,argv[1],&addr.sin_addr);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return(1);
    }

    ret = listen(fd,10);
    if(ret < 0)
    {
        perror("listen");
        return (1);
    }

    while(1)
    {
        struct sockaddr_in* client_addr;
        socklen_t len;
        int client_fd = accept(fd,(struct sockaddr*)&client_addr,&len);

        if(client_fd < 0)
        {
            perror("accept");
            continue;
        }
        
        char input_buf[1024*10] = {0};
        ssize_t read_size = read(client_fd,input_buf,sizeof(input_buf)-1);
        if(read_size < 0)
        {
            return 1;
        }
        printf("[requst] %s\n",input_buf);
        char buf[1024] = {0};
        const char* hello = "<h1>shna ji </h1>";
        sprintf(buf,"HTTP/1.0 200 OK\n Content-Length:%lu\n\n%s",
                strlen(hello),hello);
        write(client_fd,buf,strlen(buf));
    }
    return 0;
}
