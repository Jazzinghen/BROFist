#ifndef __bro_headers_bt_h
#define __bro_headers_bt_h

#include <stdint.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "bro_fist.h"


#define MAX_BT_DEVICES 255
#define BT_INQUIRY_LEN 8

typedef struct {
    char       name[248];
    bdaddr_t   mac;
} bro_bt_device_t;

size_t bro_bt_scan_devices (bro_bt_device_t * devices[MAX_BT_DEVICES]);

int bro_bt_connect_device (int * spam_sock, bdaddr_t mac_addr);

int bro_bt_client_fist (bro_fist_t * input_fist, bro_fist_t * out_fist,
                        int spam_sock);   
                                            
int bro_bt_close_connection (int spam_sock);

#endif 
