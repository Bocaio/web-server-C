#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

#define port 9090

int main() {
  printf("Program starts\n");
  int serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serversocketfd < 0) {
    printf("Creating the socket failed");
    return 1;
  }
  //   struct sockadd serverSockAddr;
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
  while (1) {
    socklen_t socketLen = sizeof(serverSockAddr);
    int clientSocketfd =
        accept(serversocketfd, (struct sockaddr *)&serverSockAddr, &socketLen);
    if (clientSocketfd < 0) {
      printf("Accepting the socket failed");
      return 1;
    }
    printf("Client connected with %d\n", clientSocketfd);
  }
  return 1;
}