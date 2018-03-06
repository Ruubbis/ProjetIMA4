
#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <unistd.h>

#include "settings.h"
#include "socketServ.h"

#define MAX_DATA 8
#define ID_VENDOR 0x0504
#define ID_PRODUCT 0x2201


int main(void){
	int sockfd;
	sockfd = initialisationServeur();
	
	int dialogue;
	#ifdef DEBUG
		printf("Attente connexion Socket\n");
	#endif
	if((dialogue=accept(sockfd, NULL, NULL))<0) {return -1;}
	#ifdef DEBUG
		printf("Connexion Socket Recue !\n");
	#endif
	return 0;
}

