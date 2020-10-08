#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smtp.h"

static const int port = 25;

int main (int argc, char *argv[]) {

  //hostname/domain as argument
  if (argc < 2) {
      printf("Please enter an domain name: ./client.out [domain.example]\n");
      return -1;
  }

  printf("# Connecting... \n");

  char* ip = hostnameToIp(argv[1]);
  // create socket and connect to it
  int socket = createSocket(ip, port);

  // connection error
  if (socket < 0) {
    printf("# Connection failed. Please check if the inserted domain name is correct and if the smtp service is enabled.\n");
    return -1;
  }

  int len = 512;
  char *data = calloc(len, sizeof(char));

  // read first response code after connection
  int ret = readResponse(socket, data, len);

  // response code != 220 means connection error, end service
  if (ret != 220) {
    printf("# Connection refused.\n");
    return -1;
  }

  // print connection is active msg
  printf("# Connection established successfully!\n\n");

  int state = 0;
  char *input;
  char *tmp;


  do {

    state = 0;
    ret = 0;

    do {

      data = calloc(len, sizeof(char));
      input = calloc(len, sizeof(char));

      switch (state) {
        //enter the sender email
        case 0: {
          // get input and send command
          if (ret == 0) {
            printf(">> Please enter your e-mail address: ");
            fgets(input, 255, stdin);

            strcpy(data, "MAIL FROM: ");
            strcat(data, input);
          }
          // cmd was sended, check response codes
          else {
            if (ret == 250)
              state++;
            else if (ret == 501)
              printf("# Invalid e-mail address.\n");
            else
              printf("# Unknown error\n");
          }
          break;
        }

        //enter the recipient email
        case 1: {
          // get input and send command
          if (ret == 0) {
            printf(">> Please enter the e-mail address of the recipient: ");
            fgets(input, 255, stdin);

            strcpy(data, "RCPT TO: ");
            strcat(data, input);
          }
          // cmd was sended, check response codes
          else {
            if (ret == 250)
              state++;
            else if (ret == 454 || ret == 501)
              printf("# Invalid e-mail address.\n");
            else if (ret == 550)
              printf("# This user does not exist.\n");
            else
              printf("# Unknown error\n");
          }
          break;
        }

        //send the data command to the server
        case 2: {
          // send command
          if (ret == 0)
            strcpy(data, "DATA\n");
          // cmd was sended, check response codes
          else {
            if (ret == 354)
              state++;
            else
              printf("# Unknown error\n");
          }
          break;
        }

        // enter the subject
        case 3: {

          printf("\n>> Enter the subject: ");
          fgets(input, 255, stdin);
          strcpy(data, "Subject: ");
          strcat(data, input);
          printf(">> Enter your message (end it by typing a '.' in an empty line):\n\n");
          state++;
          break;
        }

        // enter the message and confirm the sending
        case 4: {

          // get a line form the user
          printf(">> ");
          fgets(input, 255, stdin);
          strcpy(data, input);

          // empty line with '.' means end of the message
          if (strlen(input) == 2 && input[0] == '.') {

            printf("\n");
            // let the user confirm the sending
            do {
              // get user input (y means sending the mail, n means aborting the operation)
              printf(">> Sure you want to send this email? (Y/N): ");
              input = calloc(len, sizeof(char));
              fgets(input, 255, stdin);

              // input is valid, exit the loop
              if (input[0] == 'N' || input[0] == 'n') {
                closeSocket(socket);

                //create new socket and read first response
                socket = createSocket(ip, port);
                readResponse(socket, data, len);
                data = calloc(len, sizeof(char));

                ret = -1;
                break;
              } else if (input[0] == 'Y' || input[0] == 'y')
                break;
            } while(1);

            // end the mail sending procedure
            state = -1;
          }
          break;
        }
      }

      // send a command
      if (ret == 0) {
        //send the command
        sendCommand(socket, data, strlen(data));
        // only at the beginning read the response code
        if (state < 3) {
          ret = readResponse(socket, data, len);
          // response code < 0 means connection error
          if (ret < 0) {
            printf("# Connection error. Please try again later.\n");
            return -1;
          }
        }
      } else
        ret = 0;

    } while(state != -1);

    // user aborted the sending
    if (ret == 0)
      printf("# Your e-mail has not been sent.\n");
    // email was sended
    else if (ret == 250)
      printf("# Your e-mail has been sent successfully!\n");
    // some other error ocurred
    else
      printf("# Unexpected error during sending your email. Please try again later!\n");


    printf(">> Do you want to send another mail? (Y/N): ");
    input = calloc(len, sizeof(char));
    fgets(input, 255, stdin);

    // input is valid, exit the loop
    if (input[0] == 'N' || input[0] == 'n') {
      break;
    }
    printf("\n");

  } while(1);

  // bye message
  printf("Bye!\n");

}
