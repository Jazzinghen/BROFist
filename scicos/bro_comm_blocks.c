#include <scicos/scicos_block.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

// Include needed to have the bro_fist_t structure

#include "../../src/headers/bro_fist.h"

int sd = -1;

int bro_comm_init (scicos_block *block)
{
    int rc, bytesReceived, length;
    char   buffer[BUFFER_LENGTH];
    struct sockaddr_un serveraddr;
    bro_fist_t out_packet, in_packet;
    
    /********************************************************************/
    /* The socket() function returns a socket descriptor, which represents   */
    /* an endpoint.  The statement also identifies that the UNIX  */
    /* address family with the stream transport (SOCK_STREAM) will be   */
    /* used for this socket.                                            */
    /********************************************************************/
    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("socket() failed");
        return -1;
    }

    /********************************************************************/
    /* If an argument was passed in, use this as the server, otherwise  */
    /* use the #define that is located at the top of this program.      */
    /********************************************************************/
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    if (argc > 1)
        strcpy(serveraddr.sun_path, argv[1]);
    else
        strcpy(serveraddr.sun_path, SERVER_PATH);

    /********************************************************************/
    /* Use the connect() function to establish a connection to the      */
    /* server.                                                          */
    /********************************************************************/
    rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
    if (rc < 0)
    {
        perror("connect() failed");
        return -1;
    }

    length = sizeof(bro_fist_t) * BUFFER_SIZE;
    rc = setsockopt(sd, SOL_SOCKET, SO_RCVLOWAT,
    (char *)&length, sizeof(length));

    if (rc < 0)
    {
        perror("setsockopt(SO_RCVLOWAT) failed");
        return -1;
    }
}


void bro_comm_kill (scicos_block *block)
{
    close(sd);
}

void bro_comm_controller (scicos_block *block, int flag)
{
	switch (flag) {
		case 1:	/* set output */
		    break;
		case 2:	/* get input */
			break;
		case 4:	/* initialisation */
		    bro_comm_init(block);
	        break;
		case 5:	/* ending */
			bro_comm_kill(block);
			break;
	    default:
	        break;
	}
}
