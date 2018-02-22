#ifndef SOCKETSERV_H
	#define SOCKETSERV_H
	
	#include <libusb-1.0/libusb.h>
	#define SOCK_PATH "/tmp/local_socket_srv"
	#define BACKLOG 1
	#define MAX_MSG 8
	int initialisationServeur();
	int gestionServeur(int sockfd,int(*traitement)(int,libusb_device_handle *,int, int), libusb_device_handle * handle, int endpoint_in, int endpoint_out);
#endif
