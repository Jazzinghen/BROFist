#include <scicos/scicos_block.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// Include needed to have the bro_fist_t structure

#include "../src/headers/bro_fist.h"

void bro_encode_sci_datablock (bro_fist_t *data_block, double *params)
{
    switch ((uint8_t)params[1])
    {
        case 1:
            data_block->port = PORT_1;
            break;
        case 2:
            data_block->port = PORT_2;
            break;
        case 3:
            data_block->port = PORT_3;
            break;
        case 4:
            data_block->port = PORT_4;
            break;
        case 5:
            data_block->port = MOTOR_A;
            break;
        case 6:
            data_block->port = MOTOR_B;
            break;
        case 7:
            data_block->port = MOTOR_C;
            break;
        default:
            break;
    };
    
    switch ((uint8_t)params[2])
    {
        case 1:
            data_block->operation = TACHO_COUNT;
            break;
        case 2:
            data_block->operation = AVG_SPEED;
            break;
        case 3:
            data_block->operation = LIGHT_SENSOR;
            break;
        case 4:
            data_block->operation = TOUCH_SENSOR;
            break;
        case 5:
            data_block->operation = SOUND_SENSOR;
            break;
        case 6:
            data_block->operation = RADAR_SENSOR;
            break;
        case 7:
            data_block->operation = SET_POWER;
            break;
        case 8:
            data_block->operation = SET_SPEED;
            break;
        default:
            break;
    };
    
    data_block->data = (float)params[3];
}

void bro_decode_sci_datablock (bro_fist_t *data_block, double *param)
{
    printf("Ho ricevuto il dato: %.2f\n", data_block->data);
    *param = (double)data_block->data;
}


int bro_comm_init (scicos_block *block)
{
    int rc, length;
    // char   buffer[BUFFER_LENGTH];
    struct sockaddr_un serveraddr;
    int global_sd = -1;
    
    printf("Init. Socket\n");
    
    /********************************************************************/
    /* The socket() function returns a socket descriptor, which represents   */
    /* an endpoint.  The statement also identifies that the UNIX  */
    /* address family with the stream transport (SOCK_STREAM) will be   */
    /* used for this socket.                                            */
    /********************************************************************/
    global_sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (global_sd < 0)
    {
        perror("socket() failed");
        return -1;
    }


    printf("Linking it to the system socket\n");
    /********************************************************************/
    /* If an argument was passed in, use this as the server, otherwise  */
    /* use the #define that is located at the top of this program.      */
    /********************************************************************/
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, SERVER_PATH);
    
    printf("Connecting...\n");
    
    /********************************************************************/
    /* Use the connect() function to establish a connection to the      */
    /* server.                                                          */
    /********************************************************************/
    rc = connect(global_sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
    if (rc < 0)
    {
        perror("connect() failed");
        return -1;
    }

    printf("Done, setting connection params :3\n");
    length = sizeof(bro_fist_t) * BUFFER_SIZE;
    rc = setsockopt(global_sd, SOL_SOCKET, SO_RCVLOWAT,
    (char *)&length, sizeof(length));

    if (rc < 0)
    {
        perror("setsockopt(SO_RCVLOWAT) failed");
        return -1;
    }

    printf("Ok, all done!\n");
    
    block->outptr[0][0] = global_sd;
    
    return 0;
}


int bro_comm_kill (scicos_block *block)
{
    int rc;
    bro_fist_t packet[BUFFER_SIZE];
    
    packet[0].operation = 255;
    
    rc = send(block->inptr[0][0], &packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    
    if (rc < 0)
    {
        perror("send() failed");
        return -1;
    }
    
    close(block->outptr[0][0]);
    
    return 0;
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

int bro_sens_send (scicos_block *block)
{
    int rc, i;
    bro_fist_t packet[BUFFER_SIZE];
    
    for (i = 1; i < block->nin; i++) {
        bro_encode_sci_datablock(&packet[i-1], block->inptr[i]);
    };
    
    printf ("Data for first block: %i, %i, %.2f\n", packet[0].port, packet[0].operation, packet[0].data);
    
    rc = send(block->inptr[0][0], packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    
    if (rc < 0)
    {
        perror("send() failed");
        return -1;
    }

    printf("%d bytes of data were sent\n", rc);
    return 0;
}

int bro_sens_read (scicos_block *block)
{
    int rc, i;
    bro_fist_t packet[BUFFER_SIZE];
    
    rc = recv(block->inptr[0][0], packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    printf("%d bytes of data were received\n", rc);
    if (rc < 0)
    {
        perror("recv() failed");
        return -1;
    }
    
    printf("Starting to set outputs :3 [%i]\n", block->nout);
    
    for (i = 0; i < block->nout; i++) {
        printf("Next Step defining outputs :D [%i]\n", i);
        bro_decode_sci_datablock(&packet[i], &block->outptr[i][0]);
        printf("Output value for port %i is: %.2f\n", i, block->outptr[i][0]);
    }
    
    return 0;
}

void bro_comm_sens_disp (scicos_block *block, int flag)
{
	switch (flag) {
		case 1:	/* set output */
		    bro_sens_send(block);
		    bro_sens_read(block);
		    break;
		case 2:	/* get input */
			break;
		case 4:	/* initialisation */
	        break;
		case 5:	/* ending */
			break;
	    default:
	        break;
	}
}

int bro_motor_set (scicos_block *block)
{
    return 0;
}

void bro_comm_motor_disp (scicos_block *block, int flag)
{
	switch (flag) {
		case 1:	/* set output */
		    break;
		case 2:	/* get input */
		    bro_motor_set(block);
			break;
		case 4:	/* initialisation */
	        break;
		case 5:	/* ending */
			break;
	    default:
	        break;
	}
}
