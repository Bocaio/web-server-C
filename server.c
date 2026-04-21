#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define port 9090
#define BUFFER_SIZE 1024

void getMethod(int socketfd, char *buff, char *method) {
  int request = recv(socketfd, buff, BUFFER_SIZE, 0);
  if (request <= 0) {
    printf("Didn't receive any data in requests\n");
  }
  printf("Buffer is %s\n", buff);

  sscanf(buff, "%15s", method);
  printf("Method: %s\n", method);
}

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
    int clientSocketfd =
        accept(serversocketfd, (struct sockaddr *)&serverSockAddr, &socketLen);
    if (clientSocketfd < 0) {
      printf("Accepting the socket failed");
      return 1;
    }
    printf("Client connected with %d\n", clientSocketfd);
    char buffer[BUFFER_SIZE];
    char method[16];
    getMethod(clientSocketfd, buffer, method);
  }
  return 1;
}