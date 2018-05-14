#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#include "settings.h"
#include "Communication/libSocketServ.h"
#include "Libusb/libXbeeController.h"

void string_copy(unsigned char * src, unsigned char * dst, int size){
	int i;
	for(i=0;i<size;i++){
		dst[i] = src[i];
	}
}


int controlClient(int sockfd){
	
	#ifdef DEBUG
		printf("DEBUT CONTROL CLIENT\n");
	#endif
	unsigned char ordre[MAX_MSG] = {0};
	unsigned char * data;
	int bytes;
	while((bytes=read(sockfd,ordre,MAX_MSG))>0){
		#ifdef DEBUG
			printf("Data received : %s\n",ordre);
		#endif
		bytes--;
		data = malloc(bytes*sizeof(unsigned char));
		string_copy(ordre,data,bytes);
		#ifdef DEBUG
			printf("Bytes transferred : %d\n",bytes);
			printf("Data transferred : %s\n",data);
		#endif
		write_endpoint(data,bytes);
		free(data);
	}
	
	return 0;
}

int main(){

	if(init_xbee_controller()!=0){perror("Init Xbee Controller"); return(EXIT_FAILURE);}
	
	int sockfd = initialisationServeur();
	gestionServeur(sockfd,controlClient);	

	close_xbee_controller();
}
