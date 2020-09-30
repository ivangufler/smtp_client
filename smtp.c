#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

const int FLAGS = 0;

int createSocket(const char *ip, const int port) {

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  int value = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &value, sizeof(value));

  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_port = htons(port);
  serv_addr.sin_family = AF_INET;

  if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) > 0) {
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
      return -1;
  }
  return sock;
}

int sendCommand(int socket, char *buffer, int buflen) {
  send(socket, buffer, buflen, FLAGS);
}

int readResponse(int socket, char *buffer, int buflen) {

  int bytesread = recv(socket, buffer, buflen, FLAGS);
  if(bytesread <= 0) {
    printf("Socket was closed");
    return -1; //socket was closed
  }

  char *tmp = strchr(buffer, ' ');
  *tmp = '\0';

  return atoi(buffer);
}
