#include <stdio.h>
#include <errno.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include "headers.h"

#define SERV_PORT 7777
#define CLI_PORT 9999
#define SERV_IP "192.168.88.248"
#define CLI_IP "192.168.88.247"
#define ETHER_NAME "wlan0"
#define BUFF_SIZE 64
#define PACK_SIZE 128

int main(void)
{
	struct sockaddr_ll serv, cli;
	socklen_t len;
	int sock;
	int rc, flag = 1;	
	char buff[SERV_PORT] = "Hello server!\0";
	char *packet;
	struct iphdr *iph = NULL;
	struct udphdr *udph = NULL;
	struct ether_header *ethh = NULL;
	struct in_addr saddr;
	unsigned char mac_source[6] = {0x0c, 0x60, 0x76, 0x6e, 0x5b, 0x2f};
	unsigned char mac_dest[6] = {0xb8, 0xe8, 0x56, 0x3f, 0xb9, 0x80};

	memset(&cli, 0, sizeof(cli));
	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sock < 0) {
		perror("Create socket");
		exit(1);
	}


	len = sizeof(struct sockaddr_ll);
	rc = getsockname(sock, (struct sockaddr *) &cli, &len);
	if (rc < 0) {
		perror("Can`t get client end-point");
		close(sock);
		exit(1);
	} 

	//printf("ip - > %s port-> %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

	memset(&serv, 0, sizeof(serv));
	serv.sll_family = AF_PACKET;
	//serv.sll_protocol =htons( ETH_P_ALL);
	serv.sll_ifindex = if_nametoindex(ETHER_NAME);
	serv.sll_halen = ETH_ALEN;
	memmove((void *) (serv.sll_addr), (void *) mac_dest, ETH_ALEN);


	packet = calloc(PACK_SIZE, sizeof(char));
	memmove(packet + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr), 
		buff, strlen(buff) + 1);
	create_udp_header(packet + sizeof(struct ether_header) + sizeof(struct iphdr), SERV_PORT, 
			CLI_PORT, strlen(buff) + 1);
	create_ip_header(packet + sizeof(struct ether_header), CLI_IP, SERV_IP, sizeof(struct udphdr) + strlen(buff) + 1);
	create_mac_header(packet, mac_source, mac_dest);

	len = sizeof(struct sockaddr_ll);
	rc = sendto(sock, packet, PACK_SIZE, 0, (struct sockaddr *)&serv, len);
	if (rc < 0) {
		perror("Can`t send message to server");
	} else {
		printf("Send to server bytes -> %d\n", rc);
	}
	
	while(1) {
		//memset(&serv, 0, sizeof(struct sockaddr_in));
		memset(packet, 0, PACK_SIZE);
		len = sizeof(struct sockaddr_in);
	
		rc = recvfrom(sock, packet, PACK_SIZE, 0, (struct sockaddr *)&serv, &len);
		if (rc < 0) {
			perror("Can`t recv message from server");
		} else {
			iph = (struct iphdr *) (packet + sizeof(struct ether_header));
			udph = (struct udphdr *) (packet + sizeof(struct ether_header) + sizeof(struct iphdr));
			saddr.s_addr = iph->saddr;
			printf("Header of message ip - %s sport - %d dport - %d\n", inet_ntoa(saddr), 
					ntohs(udph->source), ntohs(udph->dest));
			printf("Recv from server message -> %s %d bytes\n", 
				packet + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr), rc);
		}
	}
	close(sock);
	return 0;
}
