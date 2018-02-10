#include "settings.h"

int main(void){
	int sockfd;
	struct sockaddr_in srv_addr;
	int sin_size;

	if((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) == -1){
		perror("Erreur création socket\n");
		exit(1);
	}
	server_addr.sin_family = AF_UNIX;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) == -1){
		perror("Erreur Bind\n");
		exit(2);
	}	

	if(listen(sockfd, BACKLOG) == -1){
		perror("Erreur Listen\n");
		exit(3);
	}		
}

