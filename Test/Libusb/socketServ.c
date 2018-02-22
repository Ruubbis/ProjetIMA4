#include "settings.h"
#include "socketServ.h"
#include <libusb-1.0/libusb.h>

int initialisationServeur(){
	struct sockaddr_un addr;
	int sockfd;
	int statut;
	//SOCKET CREATION
	
	#ifdef DEBUG
		printf("Creation Socket\n");
	#endif
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sockfd == -1){perror("Error : Creating a socket"); return EXIT_FAILURE;}
	if (remove(SOCK_PATH) == -1 && errno != ENOENT){perror("Error : deleting old socket file"); return EXIT_FAILURE;}


	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) -1);

	#ifdef DEBUG
		printf("Binding Socket\n");
	#endif
	//SOCKET BIND
	statut = bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(statut < 0){perror("Error : binding socket"); return EXIT_FAILURE;}
	//SOCKET LISTEN
	#ifdef DEBUG
		printf("Listen Socket\n");
	#endif
	statut = listen(sockfd, BACKLOG);
       	if(statut < 0){perror("Error : listening socket"); return EXIT_FAILURE;}
	return sockfd;
}

int gestionServeur(int sockfd, int (*traitement)(int, libusb_device_handle *, int, int), libusb_device_handle * handle, int endpoint_in, int endpoint_out){
	int dialogue;
	#ifdef DEBUG
		printf("Attente connexion Socket\n");
	#endif
	if((dialogue=accept(sockfd, NULL, NULL))<0) {return -1;}
	#ifdef DEBUG
		printf("Connexion Socket Recue !\n");
	#endif
	if(traitement(dialogue,handle,endpoint_in,endpoint_out) <0){	
		shutdown(sockfd, SHUT_RDWR);}
	return 0;
}



