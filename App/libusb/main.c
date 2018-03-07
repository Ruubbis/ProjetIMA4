#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <unistd.h>

#include "settings.h"
#include "socketServ.h"

#define MAX_DATA 5
#define ID_VENDOR 0x0504
#define ID_PRODUCT 0x2201

typedef struct{
	int interface;
	int address;
	int direction;
}EndPoint;


int init_context(libusb_context ** context){
	int status=libusb_init(context);
	if(status!=0){perror("libusb_init"); return -1;}
	return 0;
}


int find_target(libusb_context * context, libusb_device_handle ** handle){
	libusb_device **list;
	ssize_t count = libusb_get_device_list(context,&list);
	if(count<0) {perror("libusb_get_device_list"); exit(-1);}
	ssize_t i = 0;
	for(i=0;i<count;i++){
		libusb_device *device = list[i];
		struct libusb_device_descriptor desc;
		int status=libusb_get_device_descriptor(device,&desc);
		if(status!=0) continue; //SI ERREUR, RETOUR DEBUT DE BOUCLE
		#ifdef DEBUG
		uint8_t bus=libusb_get_bus_number(device);
		uint8_t address=libusb_get_device_address(device);
		#endif
		if(desc.idProduct == ID_PRODUCT && desc.idVendor == ID_VENDOR){
			#ifdef DEBUG
				printf("Device %x|%x Found @ (Bus:Address) %d:%d\n",ID_VENDOR, ID_PRODUCT,bus,address);
			#endif
			int success = libusb_open(device, handle);
			if(success!=0){perror("libusb_open"); return -1;}
			return 0;
		}
	}
	#ifdef DEBUG
		printf("target %x:%x not found\n",ID_VENDOR,ID_PRODUCT);
	#endif
	return -1;
}

int get_interface_number(libusb_device_handle * handle, int * configValue, int ** interfaces_list, int * nb_interface, EndPoint ** endpoint_list, int * nb_endpoint){
//Recupere le numero de l'interface utilise et le numero de la configuration du peripherique.
//Retourne 0 au succes, sinon -1
	int k;
	libusb_device * target_device = libusb_get_device(handle);
	struct libusb_config_descriptor * config;
	int endpoint_nb = 0;
	int success = libusb_get_config_descriptor(target_device,0,&config);
	if(success!=0){perror("libusb_get_config_descriptor"); return -1;}
	#ifdef DEBUG
		printf("configValue = %d\n",config->bConfigurationValue);
	#endif
	*configValue = config->bConfigurationValue;	
	
	#ifdef DEBUG
		printf("Interfaces Number : %d\n",config->bNumInterfaces);
		printf("Max Power : %d\n",config->MaxPower);
	#endif

	int N_interfaces = config->bNumInterfaces;
	struct libusb_interface interface;
	struct libusb_interface_descriptor desc_interface;
	struct libusb_endpoint_descriptor endpoint_desc;
	int tmp_interface_number;
	int n;
	*nb_interface = N_interfaces;
	*interfaces_list = (int *)malloc(N_interfaces*sizeof(int));
	for(n=0;n<N_interfaces;n++){
		interface = config->interface[n];
		desc_interface = interface.altsetting[0];
		tmp_interface_number = desc_interface.bInterfaceNumber;
		*((*interfaces_list)+n) = tmp_interface_number;
		for(k=0;k<desc_interface.bNumEndpoints;k++){
			endpoint_desc = desc_interface.endpoint[k];
			#ifdef DEBUG
				printf("Interface : %d -- Numéro : %d -- Point d'acces : %d -- Type d'acces : %d\n",n,tmp_interface_number,endpoint_desc.bEndpointAddress, endpoint_desc.bmAttributes);		
			#endif
			if(endpoint_desc.bmAttributes == 3){
				endpoint_nb++;
				*endpoint_list = realloc(*endpoint_list, endpoint_nb * sizeof(EndPoint));
				((*endpoint_list)+(endpoint_nb-1))->interface = tmp_interface_number;
				((*endpoint_list)+(endpoint_nb-1))->address = endpoint_desc.bEndpointAddress;
				((*endpoint_list)+(endpoint_nb-1))->direction = (((endpoint_desc.bEndpointAddress & LIBUSB_ENDPOINT_IN) != 0)?1:0);
			}
		}
	}
	*nb_endpoint = endpoint_nb;
	return 0;
}

int claim_interface(libusb_device_handle * handle, int interfaceNumber){
	#ifdef DEBUG
		printf("CLAIM INTERFACE : Handle = %p -- interfaceNumber = %d\n",(void *)handle,interfaceNumber);
	#endif
	int status=libusb_claim_interface(handle,interfaceNumber);
	if(status!=0){perror("libusb_claim_interface"); return -1;}
	return 0;
}

int release_interface(libusb_device_handle * handle, int interfaceNumber){
	#ifdef DEBUG
		printf("RELEASE INTERFACE : Handle = %p -- interfaceNumber = %d\n",(void *)handle,interfaceNumber);
	#endif
	int status = libusb_release_interface(handle,interfaceNumber);
	if(status!=0){perror("libusb_release_interface"); return -1;}
	return 0;
}

int claim_all_interfaces(libusb_device_handle * handle, int configValue, int * interfaces_list, int nb_interfaces){
	#ifdef DEBUG
		printf("CLAIM ALL INTERFACE : Handle = %p -- ConfigValue = %d\n",(void *)handle, configValue);
	#endif
	int i;
	int status = libusb_set_configuration(handle,configValue);
	if(status!=0){perror("libusb_set_configuration"); return -1;}
	
	for(i=0;i<nb_interfaces;i++){
		claim_interface(handle, (*interfaces_list+i));
	}
	return 0;
}

int release_all_interfaces(libusb_device_handle * handle, int * interfaces_list, int nb_interfaces){
	
	#ifdef DEBUG
		printf("RELEASE ALL INTERFACE : Handle = %p \n",(void *)handle);
	#endif
	
	int i;
	for(i=0;i<nb_interfaces;i++){
		int status = release_interface(handle, (*interfaces_list+i));
		if(status!=0){perror("release_all_interfaces"); return -1;}
	}	
	return 0;
}

int read_endpoint(libusb_device_handle * handle, int endpoint_in){

	#ifdef DEBUG
		printf("READ INTERRUPTION : Handle = %p -- Endpoint IN = %d\n",(void *)handle, endpoint_in);
	#endif

	unsigned char data;
	int transferred;
	int timeout = 5000;
	int status = libusb_interrupt_transfer(handle, endpoint_in, &data, 1, &transferred, timeout);
	if(status!=0){printf("status = %d\n",status);printf("%s\n",libusb_error_name(status)); perror("libusb_interrupt_transfer");}
	if(transferred==1){printf("data received !\n");
	printf("data = %d\n",data);}
	return 0;

}

int write_endpoint(libusb_device_handle * handle, int endpoint_out, int led_state){
	
	#ifdef DEBUG
		printf("CHANGE LED STATUS : Handle = %p -- Endpoint OU = %d\n",(void *)handle, endpoint_out);
	#endif
	
	unsigned char data;
	int transferred;
	int timeout = 1000;
	data = 0x40+led_state;
	int status = libusb_interrupt_transfer(handle, endpoint_out, &data, 1, &transferred, timeout);
	if(status!=0){printf("status = %d\n",status); perror("libusb_interrupt_transfer"); return -1; }
	return 0;
}

int release_kernel(libusb_device_handle * handle, int * interfaces_list, int nb_interfaces){ 
	#ifdef DEBUG
		printf("RELEASE KERNEL : Handle = %p\n",(void *)handle);
		printf("INTERFACES : N1 %d -- N2 %d\n",*(interfaces_list+0),*(interfaces_list+1));
	#endif
	
	int i;
	for(i=0;i<nb_interfaces;i++){	
		if(libusb_kernel_driver_active(handle,*(interfaces_list+i))){
			int status=libusb_detach_kernel_driver(handle,*(interfaces_list+i));
			if(status!=0){perror("libusb_detach_kernel_driver"); exit(-1);}
		}
	}

	#ifdef DEBUG
		printf("RELEASE KERNEL : Success\n");
	#endif
	
	return 0;
}

int controlClient(int sockfd, libusb_device_handle * handle, int endpoint_in, int endpoint_out){
	
	#ifdef DEBUG
		printf("DEBUT CONTROL CLIENT\n");
	#endif
	
	FILE * dialogue = fdopen(sockfd, "a+");
	if(dialogue == NULL) {perror("controlClient fdopen"); return(EXIT_FAILURE);}

	char ordre[MAX_MSG];
	while(fgets(ordre,MAX_MSG,dialogue) != NULL){
		#ifdef DEBUG
		printf("Data reçu : %s\n",ordre);
		#endif
		switch(ordre[0]){
			case 'A':
				switch(ordre[2]){
					case '0':
						write_endpoint(handle,endpoint_out,2);
						break;
					case '1':
						write_endpoint(handle,endpoint_out,1);
						break;
				}
				break;
			case 'B':
				switch(ordre[2]){
					case '0':
						write_endpoint(handle,endpoint_out,4);
						break;
					case '1':
						write_endpoint(handle,endpoint_out,3);
						break;
				}
				break;
		}
	}

	#ifdef DEBUG
		printf("CONTROL CLIENT FIN DE SOCKET\n");
	#endif
	fclose(dialogue);
	return 0;
}

int main(){
	int i;
	int configValue;
	EndPoint * endpoint_list = NULL;
	int nb_ep;
	int * interfaces = NULL;
	int nb_interfaces;
	libusb_device_handle * handle;
	int sockfd;

	//------------------------------
	//INITIALISATION BIBLIOTHEQUE
	
	#ifdef DEBUG
		printf("Initialisation de la bibliotheque...\n");
	#endif
	libusb_context *context;
	init_context(&context);

	//------------------------------
	//RECUPERATION DE LA POIGNEE DE LA CIBLE
	
	#ifdef DEBUG
		printf("Recuperation de la poignée de la cible...\n");
	#endif

	int status = find_target(context,&handle);
	if(status == -1){perror("find_target"); return -1;}
	
	#ifdef DEBUG
		printf("HANDLE = %p\n",(void *)handle);
	#endif

	//---------------------------------
	//RECUPERATION DE L INTERFACE D INTERRUPTION
	
	#ifdef DEBUG
		printf("Recuperation des interfaces et des points d'acces d'interruptions...\n");
	#endif

	
	get_interface_number(handle, &configValue,&interfaces, &nb_interfaces, &endpoint_list, &nb_ep);
	release_kernel(handle, interfaces, nb_interfaces);


	#ifdef DEBUG
		printf("CONFIG VALUE = %d\n",configValue);
		printf("Endpoint number : %d \n",nb_ep);
	#endif

	int endpoint_in, endpoint_out;
	for(i=0;i<nb_ep;i++){
		#ifdef DEBUG
			printf("Endpoint address : %d -- Endpoint direction : %d \n",endpoint_list[i].address,endpoint_list[i].direction);
		#endif
		if(endpoint_list[i].direction == 1){
			endpoint_in = endpoint_list[i].address;
		}

		else{
			endpoint_out = endpoint_list[i].address;
		}
	}


	//RECLAMATION DE L INTERFACE
	#ifdef DEBUG
		printf("Reclamation des interfaces...\n");
	#endif
	claim_all_interfaces(handle, configValue, interfaces, nb_interfaces);

	#ifdef DEBUG
		printf("Initialisation Serveur Socket\n");
	#endif

	sockfd = initialisationServeur();
	gestionServeur(sockfd,controlClient,handle,endpoint_in,endpoint_out);		

	//LIBERATION DES INTERFACE ET CLOTURE DU CONTEXTE USB
	#ifdef DEBUG
		printf("Liberations des interfaces...\n");
	#endif

	release_all_interfaces(handle, interfaces, nb_interfaces);
	libusb_close(handle);
	libusb_exit(context);
}
