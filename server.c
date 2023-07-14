#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int status, sockfd;
  struct addrinfo *res, hints;
  char *port = "9998";

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  status = getaddrinfo(NULL, port, &hints, &res);
  if (status == -1) {
    fprintf(stderr, "unable to get host information\n");
    return -1;
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd == -1) {
    fprintf(stderr, "failed to create socket\n");
    return -1;
  }

  status = bind(sockfd, res->ai_addr, res->ai_addrlen);
  if (status == -1) {
    fprintf(stderr, "failed to bind socket\n");
    return -1;
  }

  status = listen(sockfd, 10);
  if (status == -1) {
    fprintf(stderr, "failed to listen to socket\n");
    return -1;
  }

  struct sockaddr client;
  socklen_t client_size = sizeof client;
  int client_fd;
  char request[1024];
  while (1) {
    client_fd = accept(sockfd, &client, &client_size);
    if (client_fd == -1) {
      fprintf(stderr, "failed to accept connection\n");
      return -1;
    }
    status = recv(client_fd, request, 1024, 0);
    if (status == -1) {
      fprintf(stderr, "failed to recieve request\n");
      return -1;
    }
    printf("%s\n", request);
    close(client_fd);
  }

  return 0;
}
