/*
 Copyright (C) 2012 Radoslaw Tomaszewski
 License: GPL

 Description:
 Listens on UDP socket and builds up the message payload by parsing the UDP source port values from the incoming UDP packets.
 
 Execution:
  ./server
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define LISEN_PORT 5555

int main(int argc, char *argv) {
    struct protoent *ppe;
    struct sockaddr_in sin;
    char tmp[2000];
    char buf[2000]; //network buffor 

    int s, type;
    int is_continue = 1, nr; // loop condition, 1 yes, other no 

    struct sockaddr_in fsin;
    int sock_len = sizeof(fsin);
    
    unsigned char msg[2000];
    int index=0;
    int i,j;
    int port;
    
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

    /* Map service name to port number */
    sin.sin_port = htons(LISEN_PORT);
    type = SOCK_DGRAM;
    ppe = getprotobyname("udp");

    // Allocate a socket 
    s = socket(PF_INET, type, ppe->p_proto);
    if(s < 0) {
        sprintf(tmp, "can't create socket: %s\n", strerror(errno));
        perror(tmp);
    }
	
	printf("created a UPD socket on port %d\n", LISEN_PORT);
	
    // Bind the socket 
    if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
	  sprintf(tmp, "can't bind to socket: %s\n", strerror(errno));
      perror(tmp);
	}
	
	printf("listening on our UPD socket\n");

    index=0;
    while ( is_continue ) {
         if ( (nr=recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&fsin, &sock_len)) < 0) {
            sprintf(tmp, "error in recvfrom func: %s\n", strerror(errno));
            perror(tmp);
            is_continue=0;
         } else {
            buf[nr]=0;
            printf("received from ip: %s port: hex 0x%x dec %d\n" , inet_ntoa(fsin.sin_addr), ntohs(fsin.sin_port), ntohs(fsin.sin_port));
            //printf("recived data:  %s\n" , buf);
            
            port=ntohs(fsin.sin_port);
            msg[index]=(port-53)/100;
            index++;

            if ( 53 == port ) {
                is_continue=0;
                index--;
            }
        }
    }
    
    printf("Received %d characters:\n" , index);

    for(i=0; i<index; i++) {
        printf("msg[%d]=char %c hex 0x%x dec %d\n", i, msg[i], msg[i], msg[i] );
    }    
    
    return 0;
}
