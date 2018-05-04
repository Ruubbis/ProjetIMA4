#include "settings.h"
#include "socketClient.h"
#include <netinet/in.h>
#include <arpa/inet.h>

int connexionServeur(){
//	struct sockaddr_un addr;
	struct sockaddr_in addr;
	int sockfd;
	int statut;
	//SOCKET CREATION
	
	#ifdef DEBUG
	printf("Creation Socket\n");
	#endif
//	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1){perror("Error : Creating a socket"); return EXIT_FAILURE;}

/*
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) -1);
*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4000);
	inet_aton("127.0.0.1",(struct in_addr *) &addr.sin_addr.s_addr);


	#ifdef DEBUG
		printf("Connecting Socket\n");
	#endif
	statut = connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
       	if(statut < 0){perror("Error : listening socket"); return EXIT_FAILURE;}
	return sockfd;
}



int main(void){
	int statut;
	#ifdef DEBUG
		printf("Tentative Connexion Socket\n");
	#endif
	statut = connexionServeur();
	if(statut < 0){printf("Erreur de connexion\n"); return EXIT_FAILURE;}
	
	#ifdef DEBUG
		printf("Connexion accomplie\n");
	#endif

	char msg[8];
	while(1){
		int taille = read(0,msg,8);
		if(taille <= 0) break;
		write(statut,msg,taille);
		
	}

	shutdown(statut,SHUT_RDWR);
	return 0;
}

