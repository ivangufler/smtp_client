#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

const int FLAGS = 0;

int createSocket(const char *ip, const int port) {

  // create a socket with ip4 protocol, with tcp and ip
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  int value = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &value, sizeof(value));

  // set port of the connection
  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_port = htons(port);
  serv_addr.sin_family = AF_INET;

  // set ip
  if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) > 0) {
    // connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
      return -1;
  }
  // return the socket
  return sock;
}

char* hostnameToIp(char* hostname) {

	struct hostent *he;
	struct in_addr **addr_list;

	if ((he = gethostbyname(hostname)) != NULL)	{
    addr_list = (struct in_addr **) he->h_addr_list;

    for(int i = 0; addr_list[i] != NULL; i++) {
      //Return the first one
      return inet_ntoa(*addr_list[i]);
    }
	}
	return "";
}

void closeSocket(int sock) {
  close(sock);
}

int sendCommand(int socket, char *buffer, int buflen) {
  // send the string buffer with buflen to the socket
  send(socket, buffer, buflen, FLAGS);
}

int readResponse(int socket, char *buffer, int buflen) {

  // recive a response from the socket and write it to buffer
  int bytesread = recv(socket, buffer, buflen, FLAGS);
  if(bytesread <= 0) {
    //socket was closed
    return -1;
  }

  // parse response code from the recived line
  char *tmp = strchr(buffer, ' ');
  *tmp = '\0';

  // return the response code as number
  return atoi(buffer);
}
