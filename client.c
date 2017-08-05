#include <stdio.h>
#include <errno.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_PORT 7777
#define BUFF_SIZE 64

int main(void)
{
	struct sockaddr_in serv, cli;
	socklen_t len;
	int sock;
	int rc;	
	char buff[SERV_PORT] = "Hello server!\0";

	memset(&cli, 0, sizeof(cli));
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("Create socket");
		exit(1);
	}

	len = sizeof(struct sockaddr_in);
	rc = getsockname(sock, (struct sockaddr *) &cli, &len);
	if (rc < 0) {
		perror("Can`t get client end-point");
		exit(1);
	} 

	printf("ip - > %s port-> %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(SERV_PORT);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");

	rc = sendto(sock, buff, BUFF_SIZE, 0, (struct sockaddr *)&serv, len);
	if (rc < 0) {
		perror("Can`t send message to server");
	} else {
		printf("Send to server bytes -> %d\n", rc);
	}

	memset(&serv, 0, sizeof(struct sockaddr_in));
	memset(&buff, 0, BUFF_SIZE);
	rc = recvfrom(sock, buff, BUFF_SIZE, 0, (struct sockaddr *)&serv, &len);
	if (rc < 0) {
		perror("Can`t recv message from server");
	} else {
		printf("Recv from server bytes -> %d answer -> %s\n", rc, buff);
	}
	sleep(5);
	close(sock);
	return 0;
}
