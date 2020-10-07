# SMTP Client

A simple smtp client in C the programming language.
The client is started in the command line with the IPv4 address of the SMTP server as argument.
The client connects to the SMTP server via port 25 and then allows to send e-mails to this server.

The user must specify the sender address, the destination address, the subject and the text of the message. If an input is invalid, an error message is displayed.
At the end the user has the option of sending the e-mail or dropping it.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for testing purposes.

### Prerequisites

What things you need to install the software

* GCC GNU Compiler
* GNU Make

and how to install them on Linux/OSX

```
sudo apt install gcc
sudo apt install make
```

### Installing

The repo contains all necessary files, you only have to compile the C-files and then run the executables.

For getting the executable for your machine, perform the following step:

```
make
```

To run the client, perform:

```
./client.out [IPv4_OF_THE_SERVER]
```

If you want to recompile the project, use the following command before running the above command again:

```
make clean
```

## Authors

* **Ivan Gufler** - [github/ivangufler](https://github.com/ivangufler)
* **Dominik Hofer** - [github/infodo](https://github.com/infodo)

See the list of [contributors](https://github.com/ivangufler/smtp_client/contributors) who participated in this project.

## License

This project is licensed under the GPL License - Feel free to use, study, change and share.

## Acknowledgments

* This project was created for a task assignment in the subject Systems and Networks.
* TFO "Max Valier" Bolzano/Bozen, 5th year
