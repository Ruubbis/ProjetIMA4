#include "settings.h"
#include "socketClient.h"

int connexionServeur(){
	struct sockaddr_un addr;
	int sockfd;
	int statut;
	//SOCKET CREATION
	
	#ifdef DEBUG
		printf("Creation Socket\n");
	#endif
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sockfd == -1){perror("Error : Creating a socket"); return EXIT_FAILURE;}


	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) -1);

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
	return 0;
}

