#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

int main() {
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[HOSTLEN];
    int sock_id, sock_fd;
    FILE *sock_fp;
    char *ctime();
    time_t thetime;

    // 1. Построение сокетов

    sock_id = socket(PF_INET, SOCK_STREAM, 0); // create socket
    if (sock_id == -1) {
        oops("socket");
    }


    // 2. Связывание адреса с сокетом. Адрес - это: хост, порт

    bzero((void*)&saddr, sizeof(saddr)); // clear struct
    gethostname(hostname, HOSTLEN); // where am I

    hp = gethostbyname(hostname); // get info from host
    bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length); //fill host
    saddr.sin_port = htons(PORTNUM); // fill socket 
    saddr.sin_family = AF_INET;
    if (bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) { // назначаем адрес для сокета
        oops("bind");
    }
    // 3. Готовность принимать запросы на сокете в очереди с Qsize=1

    if (listen(sock_id, 1) != 0) {
        oops("listen");
    }
    // Основной цикл accepr(), write(), close()

    while (1) {
        sock_fd = accept(sock_id, NULL, NULL); // pending request
        printf("Wow, got a call!!\n");
        if (sock_fd == -1) {
            oops("accept");
        }
        sock_fp = fdopen(sock_fd, "w"); // write()
        if (sock_fp == NULL) {
            oops("fdopen");
        }
        thetime = time(NULL);
        fprintf(sock_fp, "There time here is ...");
        fprintf(sock_fp, "%s", ctime(&thetime));
        fclose(sock_fp);
    }
}