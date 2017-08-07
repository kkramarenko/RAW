#include <netinet/ip.h>
#include <arpa/inet.h>

static unsigned short in_cksum(unsigned short* addr, int len)
{
	int nleft = len;
	int sum = 0;
	unsigned short* w = addr;
	unsigned short answer = 0;

	while(nleft > 1) {
		sum += *w ++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char*) (&answer) = *(unsigned char*) w;
		sum += answer;
	}
	
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	answer = ~sum;

	return answer;	
}	

void create_ip_header(char *packet, char *source, char *dest, short tolen)
{
	struct iphdr *iph;

	iph = (struct iphdr *) packet;
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = htons((iph->ihl * 4) + tolen);
	iph->id = htons(12345);
	iph->frag_off = 0;
	iph->ttl = 64;
	iph->protocol = IPPROTO_UDP;
	iph->saddr = inet_addr(source);
	iph->daddr = inet_addr(dest);
	iph->check = (unsigned short) 0;
	iph->check = (unsigned short) in_cksum((unsigned short*)iph, iph->ihl * 4);
}

	
