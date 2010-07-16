#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH         "/tmp/BROFist"
#define BRO_MAX_CONNECTIONS 1

int bro_start_server (int * server_sock, int * client_sock)
{
    // Variable definitions
    int    rc, length;
    struct sockaddr_un bro_server;

    // Initializing socket for server
    *server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (*server_sock < 0)
    {
     perror("socket() failed");
     return -1;
    }

    // Now we'll get a unique name for our server binding the socket
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, SERVER_PATH);

    rc = bind(*server_sock, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
    if (rc < 0)
    {
     perror("bind() failed");
     return -1;
    }

    // We'll listen to connections up to a max of BRO_MAX_CONNECTIONS
    rc = listen(*server_sock, BRO_MAX_CONNECTIONS);
    if (rc< 0)
    {
     perror("listen() failed");
     return -1;
    }

    printf("Ready for client connect().\n");

    /*
     * The server uses the accept() function to accept an incoming
     * connection request.  The accept() call will block indefinitely
     * waiting for the incoming connection to arrive.
     */
    *client_sock = accept(*server_sock, NULL, NULL);
    if (*client_sock < 0)
    {
     perror("accept() failed");
     return -1;
    }

    /* We will use the SO_RCVLOWAT socket option to specify that we don't want 
     * recv() to wake up until all sizeof(bro_fist_t) of data have arrived.
     */
    length = sizeof(bro_fist_t);
    rc = setsockopt(*client_sock, SOL_SOCKET, SO_RCVLOWAT,
                                      (char *)&length, sizeof(length));
    if (rc < 0)
    {
     perror("setsockopt(SO_RCVLOWAT) failed");
     return -1;
    }
}

int bro_server_fist (){

    /*
     * TODO: Prendere in input socket di SciCos e dell'NXT ed implementare la    
     * comunicazione tra i due QUI dentro
     */
     
    /****************************************************/
    /* Receive that 250 bytes data from the client */
    /****************************************************/
    rc = recv(*client_sock, buffer, sizeof(bro_fist_t), 0);
    if (rc < 0)
    {
     perror("recv() failed");
     break;
    } 
    printf("%d bytes of data were received\n", rc);
    if (rc == 0 ||
      rc < sizeof(buffer))
    {
     printf("The client closed the connection before all of the\n");
     printf("data was sent\n");
     break;
    }

    /********************************************************************/
    /* Echo the data back to the client                                 */
    /********************************************************************/
    rc = send(*client_sock, buffer, sizeof(buffer), 0);
    if (rc < 0)
    {
     perror("send() failed");
     break;
    }

}

int bro_stop_server (int server_sock, int client_sock){
    /***********************************************************************/
   /* Close down any open socket descriptors                              */
   /***********************************************************************/
   if (server_sock != -1)
      close(server_sock);

   if (client_sock != -1)
      close(client_sock);

    /***********************************************************************/
   /* Remove the UNIX path name from the file system                      */
   /***********************************************************************/
   unlink(SERVER_PATH);
}
