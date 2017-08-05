#include <netinet/udp.h>
#include <arpa/inet.h>

void create_udp_header(char *packet, short dport, short sport, short datalen)
{
	struct udphdr *udph;

	udph = (struct udphdr *) packet;
	udph->source = htons(sport);
	udph->dest = htons(dport);
	udph->check = 0;
	udph->len = htons(datalen + sizeof(struct udphdr));
}

	
