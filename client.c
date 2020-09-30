#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smtp.h"

/*
int main() {



  const int FLAGS = 0;

  printf("Läuft\n");

  char data[1024] = { 0 };
  //receive response code
  int bytesread = recv(sock, data, sizeof(data), FLAGS);
  if(bytesread <= 0) {
    printf("Socket was closed");
    return 0; //socket was closed
  }
  printf("%s", data);

  char buffer[] = "HELO\n";
  send(sock, buffer, strlen(buffer), FLAGS);

  memset(data, 0, strlen(data));

  bytesread = recv(sock, data, sizeof(data), FLAGS);
  printf("%s", data);

  //print ("Bitte E-Mail Adresse eingeben:")
}
*/

static const char ip[] = "164.68.110.182";
static const int port = 25;

int main() {

  int socket = createSocket(ip, port);

  if (socket < 0) {
    //fuck
    return -1;
  }

  int len = 512;
  char *data = calloc(len, sizeof(char));

  int ret = readResponse(socket, data, len);

  if (ret != 220) {
    printf("Connection refused.\n");
    return -1;
  }

  printf("Connection established successfully!\n\n");

  int state = 0;
  char *input;
  char *tmp;

  ret = 0;

  while(1) {

    data = calloc(len, sizeof(char));
    input = calloc(len, sizeof(char));

    switch (state) {
      //enter sender email
      case 0: {

        if (ret == 0) {
          printf("Please enter your e-mail address: ");
          fgets(input, 255, stdin);

          strcpy(data, "MAIL FROM: ");
          strcat(data, input);
        }
        else {
          if (ret == 250)
            state++;
          else if (ret == 501)
            printf("Invalid e-mail address.\n");
          else
            printf("Unkow error");
        }
        break;
      }
      //enter recipients
      case 1: {

        if (ret == 0) {
          printf("Please enter the e-mail address of the recipient: ");
          fgets(input, 255, stdin);

          strcpy(data, "RCPT TO: ");
          strcat(data, input);
        }
        else {
          if (ret == 250)
            state++;
          else if (ret == 454)
            printf("Invalid e-mail addresssss.\n");
          else if (ret == 501)
            printf("Invalid e-mail address.\n");
          else if (ret == 550)
            printf("This user does not exist.\n");
          else
            printf("Unkown error");
        }
        break;
      }
      //enter Subject
      case 3: {

        char subject = calloc(len, sizeof(char));

        if (ret == 0) {
          printf("Please enter the subject: ");
          fgets(subject, 255, stdin);

          strcpy(data, "RCPT TO: ");
          strcat(data, input);
        }
        else {
          if (ret == 250)
            state++;
          else if (ret == 454)
            printf("Invalid e-mail addresssss.\n");
          else if (ret == 501)
            printf("Invalid e-mail address.\n");
          else if (ret == 550)
            printf("This user does not exist.\n");
          else
            printf("Unkow error");
        }
        break;
      }
    }

    if (ret == 0) {
      sendCommand(socket, data, strlen(data));
      ret = readResponse(socket, data, len);
      printf("%i\n", ret);
    } else {
      ret = 0;
    }

  }

}
