#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_PORT 7777
#define BUFF_SIZE 64

int main(void)
{
	int sock;
	int rc;
	struct sockaddr_in serv, cli;
	socklen_t len;
	char buff[BUFF_SIZE];
	char answer[12] = "Hi, client!\0";
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("Can`t create server socket");
		exit(1);
	}

	len = sizeof(struct sockaddr_in);
	memset(&serv, 0, sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(SERV_PORT);
	serv.sin_addr.s_addr  = INADDR_ANY;

	rc = bind(sock, (struct sockaddr *)&serv, len);
	if (rc < 0) {
		perror("Can`t  bind end-point to server");
		exit(1);
	}

	while(1) {
		memset(&cli, 0, sizeof(struct sockaddr_in));
		len = sizeof(struct sockaddr_in);
		rc = recvfrom(sock, buff, BUFF_SIZE, 0, (struct sockaddr *) &cli, &len);
		if (rc < 0) {
			perror("Can`t recv message from client");
		}
		
		printf("Client ip - %s port - %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
		printf("Recv form client %d bytes -> %s\n", rc, buff);

		rc = sendto(sock, answer, 12, 0, (struct sockaddr *) &cli, len);
		if (rc < 0) {
			perror("Can`t send message to client");
		}
		printf("Send to client answer \"%s\" with length - %d bytes\n", answer, rc);
	}

	return 0;
}

