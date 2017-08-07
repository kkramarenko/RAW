void create_mac_header(char *packet, unsigned char* source, unsigned char* dest);
void create_ip_header(char *packet, char *source, char *dest, short tolen);
void create_udp_header(char *packet, short dport, short sport, short tolen);
