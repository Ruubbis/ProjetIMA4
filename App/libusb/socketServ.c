#include "settings.h"
#include "socketServ.h"
#include <libusb-1.0/libusb.h>
#include <netinet/in.h>

int initialisationServeur(){
	struct sockaddr_in addr;
	int sockfd;
	int statut;
	//SOCKET CREATION
	
	#ifdef DEBUG
		printf("Creation Socket\n");
	#endif

 	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1){perror("Error : Creating a socket"); return EXIT_FAILURE;}

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr=INADDR_ANY;
	addr.sin_port=htons(9000);

	int vrai=1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
		perror("Error : Initialisation Serveur Generique REUSEADDR");
		return EXIT_FAILURE;
	}


	#ifdef DEBUG
		printf("Binding Socket\n");
	#endif
	//SOCKET BIND
	statut = bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
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



