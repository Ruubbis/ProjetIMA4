#ifndef _LIBXBEECONTROLLER_H_
#define _LIBXBEECONTROLLER_H_

#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_DATA 5
#define ID_VENDOR 0x0504
#define ID_PRODUCT 0x2201


int init_context();
int find_target();
int get_interface_number();
int claim_interface(int interfaceNumber);
int release_interface(int interfaceNumber);
int claim_all_interfaces();
int release_all_interfaces();
int read_endpoint();
int write_endpoint(int led_state);
int release_kernel();
int init_xbee_controller(void);
int close_xbee_controller(void);

#endif
