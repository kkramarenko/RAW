#include <netinet/if_ether.h>
#include <arpa/inet.h>

void create_mac_header(char *packet, unsigned char* source, unsigned char* dest)
{
	struct ether_header* ethh;
	int indx = 0;

	ethh = (struct ether_header*) packet;
	for (indx = 0; indx < 6; indx ++)
		ethh->ether_shost[indx] = source[indx];
	

	for (indx = 0; indx < 6; indx ++)
		ethh->ether_dhost[indx] = dest[indx];

	ethh->ether_type = htons(ETHERTYPE_IP);
}

	
