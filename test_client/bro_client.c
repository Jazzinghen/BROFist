/**************************************************************************/
/* This sample program provides code for a client application that uses     */
/* AF_UNIX address family                                                 */
/**************************************************************************/
/**************************************************************************/
/* Header files needed for this sample program                            */
/**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "../src/headers/bro_fist.h"

/**************************************************************************/
/* Constants used by this program                                         */
/**************************************************************************/
#define SERVER_PATH      "/tmp/BROFist"
#define BUFFER_LENGTH    250
#define FALSE              0

/* Pass in 1 parameter which is either the */
/* path name of the server as a UNICODE    */
/* string, or set the server path in the   */
/* #define SERVER_PATH which is a CCSID    */
/* 500 string.                             */
int main(int argc, char *argv[])
{
    /***********************************************************************/
    /* Variable and structure definitions.                                 */
    /***********************************************************************/
    int    sd=-1, rc, bytesReceived, length, i;
    char   buffer[BUFFER_LENGTH];
    struct sockaddr_un serveraddr;
    bro_fist_t out_packet[BUFFER_SIZE];
    bro_fist_t in_packet[BUFFER_SIZE];

    /***********************************************************************/
    /* A do/while(FALSE) loop is used to make error cleanup easier.  The   */
    /* close() of the socket descriptor is only done once at the very end  */
    /* of the program.                                                     */
    /***********************************************************************/
    do
    {
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
            break;
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
            break;
        }

        length = sizeof(bro_fist_t);
        rc = setsockopt(sd, SOL_SOCKET, SO_RCVLOWAT,
        (char *)&length, sizeof(length));

        if (rc < 0)
        {
            perror("setsockopt(SO_RCVLOWAT) failed");
            break;
        }

        out_packet[0].operation = RADAR_SENSOR;
        out_packet[0].port = PORT_1;
        out_packet[1].operation = LIGHT_SENSOR;
        out_packet[1].port = PORT_2;
        out_packet[2].operation = TOUCH_SENSOR;
        out_packet[2].port = PORT_3;

        do {
            rc = send(sd, out_packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
            if (rc < 0)
            {
                perror("send() failed");
                break;
            }
            
            printf("%d bytes of data were sent\n", rc);

            rc = recv(sd, in_packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
            printf("%d bytes of data were received\n", rc);
            if (rc < 0)
            {
                perror("recv() failed");
                break;
            }

            printf ("Received %d bytes\n", rc); 
            
            for (i = 0; i < BUFFER_SIZE; i++) {
                printf("%d Packet:\n", i+1);
                printf("\tOperation: %d\n", in_packet[i].operation);
                printf("\tPort: %d\n", in_packet[i].port);
                printf("\tData: %.2f\n\n", in_packet[i].data);
            }
        } while (1);

        out_packet[0].operation = BRO_END_COMMUNICATION;

        rc = send(sd, out_packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
        if (rc < 0)
        {
            perror("send() failed");
            break;
        }

        } while (FALSE);

    /***********************************************************************/
    /* Close down any open socket descriptors                              */
    /***********************************************************************/
    if (sd != -1)
    close(sd);
}
