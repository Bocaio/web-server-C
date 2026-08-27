
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define port 9090
#define BUFFER_SIZE 1024

void getMethod(int *socketfd, char *buff, char *method);
void *handleRequest(void *arg);

int main() {
  printf("Program starts\n");
  int serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serversocketfd < 0) {
    printf("Creating the socket failed");
    return 1;
  }
  struct sockaddr_in serverSockAddr;
  serverSockAddr.sin_family = AF_INET;
  serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverSockAddr.sin_port = htons(port);
  if (bind(serversocketfd, (struct sockaddr *)&serverSockAddr,
           sizeof(serverSockAddr)) < 0) {
    printf("Binding the socket failed");
    return 1;
  }
  if (listen(serversocketfd, 10) < 0) {
    printf("Listening the socket failed");
    return 1;
  }
  printf("Server Listening on port:%d\n", port);
  while (1) {
    socklen_t socketLen = sizeof(serverSockAddr);
    int newSocketfd =
        accept(serversocketfd, (struct sockaddr *)&serverSockAddr, &socketLen);
    if (newSocketfd < 0) {
      printf("Accepting the socket failed");
      return 1;
    }
    printf("Client connected with %d\n", newSocketfd);
    pthread_t thread1;
    pthread_create(&thread1, NULL, handleRequest, &newSocketfd);
    printf("It is running in main thread\n");
  }
  return 1;
}

void getMethod(int *socketfd, char *buff, char *method) {
  int request = recv(*socketfd, buff, BUFFER_SIZE, 0);
  if (request <= 0) {
    printf("Didn't receive any data in requests\n");
  }
  printf("Buffer is %s\n", buff);

  sscanf(buff, "%15s", method);
  printf("Method: %s\n", method);
}

void *handleRequest(void *arg) {
  // this will return after sending response
  int socketfd = *(int *)arg;
  char buffer[BUFFER_SIZE];
  char method[16];
  getMethod(&socketfd, buffer, method);
  char responseBuff[BUFFER_SIZE];
  strcpy(responseBuff,
         "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "
         "13\r\n\r\nHello, World!");
  int response = send(socketfd, responseBuff, strlen(responseBuff), 0);
  printf("Responsed %d\n", response);
  sleep(5);
  return NULL;
}