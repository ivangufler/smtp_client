#ifndef SMTP_H
#define SMTP_H

  int createSocket(const char *ip, const int port);
  char* hostnameToIp(char* hostname);
  void closeSocket(int sock);
  int sendCommand(int socket, char *buffer, int buflen);
  int readResponse(int socket, char *buffer, int buflen);

#endif
