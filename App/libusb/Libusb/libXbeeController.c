/**
 * \file libXbeeController.c
 * \brief Librairie USB Xbee
 * \author Delobelle Matthieu
 * \version 1.0
 * \date 9 mai 2018
 *
 * Librairie d'utilisation de la cle USB Xbee, regroupe les fonctions d'initialisation, de fermeture de la clé et d'envoie reception de message
 */

#include "libXbeeController.h"


int configValue;
int * interfaces_list = NULL;
int nb_interfaces;
libusb_device_handle * handle;
int sockfd;
libusb_context *context;
int endpoint_in, endpoint_out;

int init_context(){
	/**
	 * \fn int init_context ( void )
	 * \brief Fonction d'initialisation du contexte USB
	 *
	 * \param Aucun
	 * \return 0 au succes, -1 à l'echec
	 */
	int status=libusb_init(&context);
	if(status!=0){perror("init_context.libusb_init"); return -1;}
	return 0;
}


int find_target(){
	/**
	 * \fn int find_target ( void )
	 * \brief Fonction permettant d'obtenir la poignée vers le produit cible
	 *
	 * \param Aucun, l'identifiant du produit et du vendeur sont definis dans le .h
	 * \return 0 au succes, -1 a l'echec
	 */
	libusb_device ** list;
	ssize_t count = libusb_get_device_list(context,&list);
	if(count<0) {perror("find_target.libusb_get_device_list"); exit(-1);}
	ssize_t i = 0;
	for(i=0;i<count;i++){
		libusb_device * device = list[i];
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
			int success = libusb_open(device, &handle);
			if(success!=0){perror("find_target.libusb_open"); return -1;}
			return 0;
		}
	}
	#ifdef DEBUG
		printf("target %x:%x not found\n",ID_VENDOR,ID_PRODUCT);
	#endif
	return -1;
}

int get_interface_number(){
	/**
	 * \fn int get_interface_number ( void )
	 * \brief Recupere le numero de l'interface utilisee et le numero de la configuration du peripherique.
	 *
	 * \param Aucun
	 * \return 0 au succes, -1 a l'echec
	 */
	int k;
	libusb_device * target_device = libusb_get_device(handle);
	struct libusb_config_descriptor * config;
	int success = libusb_get_config_descriptor(target_device,0,&config);

	if(success!=0){perror("get_interface_number.libusb_get_config_descriptor"); return -1;}
	#ifdef DEBUG
		printf("configValue = %d\n",config->bConfigurationValue);
	#endif
	configValue = config->bConfigurationValue;	
	
	#ifdef DEBUG
		printf("Interfaces Number : %d\n",config->bNumInterfaces);
		printf("Max Power : %d\n",config->MaxPower);
	#endif

	nb_interfaces = config->bNumInterfaces;
	
	struct libusb_interface interface;
	struct libusb_interface_descriptor desc_interface;
	struct libusb_endpoint_descriptor endpoint_desc;
	int n;
	
	interfaces_list = (int *)malloc(nb_interfaces*sizeof(int));
	for(n=0;n<nb_interfaces;n++){
		interface = config->interface[n];
		desc_interface = interface.altsetting[0];
		interfaces_list[n] = desc_interface.bInterfaceNumber;
		for(k=0;k<desc_interface.bNumEndpoints;k++){
			endpoint_desc = desc_interface.endpoint[k];
			#ifdef DEBUG
				printf("Interface : %d -- Numéro : %d -- Point d'acces : %d -- Type d'acces : %d\n",n,interfaces_list[n],endpoint_desc.bEndpointAddress, endpoint_desc.bmAttributes);		
			#endif
			if(endpoint_desc.bmAttributes == 3){
				if((endpoint_desc.bEndpointAddress & LIBUSB_ENDPOINT_IN) != 0) endpoint_in = endpoint_desc.bEndpointAddress;
				else{endpoint_out = endpoint_desc.bEndpointAddress;}
			}
		}
	}
	return 0;
}

int claim_interface(int interfaceNumber){
	/**
	 * \fn int claim_interface ( int interfaceNumber )
	 * \brief Permet de \"reclamer\" (activer) l'interface choisie en parametre du peripherique
	 *
	 * \param interfaceNumber, valeur entiere positive
	 * \return 0 au succes, -1 a l'echec
	 */
	#ifdef DEBUG
		printf("CLAIM INTERFACE : Handle = %p -- interfaceNumber = %d\n",(void *)handle,interfaceNumber);
	#endif
	int status=libusb_claim_interface(handle,interfaceNumber);
	if(status!=0){perror("libusb_claim_interface"); return -1;}
	return 0;
}

int release_interface(int interfaceNumber){
	/**
	 * \fn int release_interface ( int interfaceNumber )
	 * \brief Fonction miroir a claim_interface. Permet de rendre l'interface du peripherique chosiie en parametre
	 *
	 * \param interfaceNumber, valeur entiere positive
	 * \return 0 au succes, -1 a l'echec
	 */
	#ifdef DEBUG
		printf("RELEASE INTERFACE : Handle = %p -- interfaceNumber = %d\n",(void *)handle,interfaceNumber);
	#endif
	int status = libusb_release_interface(handle,interfaceNumber);
	if(status!=0){perror("libusb_release_interface"); return -1;}
	return 0;
}

int claim_all_interfaces(){
	/**
	 * \fn int claim_all_interfaces ( void )
	 * \brief Itere la fonction claim_interface pour l'integralite des interfaces du peripheriques
	 *
	 * \param Aucun
	 * \return 0 au succes, -1 a l'echec
	 */
	#ifdef DEBUG
		printf("CLAIM ALL INTERFACE : Handle = %p -- ConfigValue = %d\n",(void *)handle, configValue);
	#endif
	int i;
	int status = libusb_set_configuration(handle,configValue);
	if(status!=0){perror("claim_all_interfaces.libusb_set_configuration"); return -1;}
	
	for(i=0;i<nb_interfaces;i++){
		if((status = claim_interface(*interfaces_list+i))!=0){perror("claim_all_configuration"); return -1;}
	}
	return 0;
}

int release_all_interfaces(){
	/**
	 * \fn int release_all_interfaces ( void )
	 * \brief Itere la fonction release_interface pour l'integralite des interfaces du peripheriques
	 *
	 * \param Aucun
	 * \return 0 au succes, -1 a l'echec
	 */

	#ifdef DEBUG
		printf("RELEASE ALL INTERFACE : Handle = %p \n",(void *)handle);
	#endif
	
	int i, status;
	for(i=0;i<nb_interfaces;i++){
		status = release_interface((*interfaces_list+i));
		if(status!=0){perror("release_all_interfaces"); return -1;}
	}	
	free(interfaces_list);
	return 0;
}

int read_endpoint(){
	/**
	 * TODO
	 */

	#ifdef DEBUG
		printf("READ INTERRUPTION : Handle = %p -- Endpoint IN = %d\n",(void *)handle, endpoint_in);
	#endif

	unsigned char data;
	int transferred;
	int timeout = 5000;
	int status = libusb_interrupt_transfer(handle, endpoint_in, &data, 1, &transferred, timeout);
	if(status!=0){printf("status = %d\n",status);printf("%s\n",libusb_error_name(status)); perror("read_endpoint.libusb_interrupt_transfer");}
	if(transferred==1){printf("data received !\n");
	printf("data = %d\n",data);}
	return 0;

}

int write_endpoint(unsigned char * data, int size){
	/**
	 * \fn int write_endpoint ( unsigned char * data, int size )
	 * \brief Envoie le message data d'une taille size via la liaison USB sur le endpoint de sortie
	 *
	 * \param data, chaine de caractère non signé représentant le message a envoyé -- size, taille du message a envoyé
	 * \return 0 au succes, -1 a l'echec
	 */

	#ifdef DEBUG
		printf("CHANGE LED STATUS : Handle = %p -- Endpoint OU = %d\n",(void *)handle, endpoint_out);
	#endif
	
	int transferred;
	int timeout = 1000;
	int status = libusb_interrupt_transfer(handle, endpoint_out, data, size, &transferred, timeout);
	if(status!=0){printf("status = %d\n",status); perror("write_endpoint.libusb_interrupt_transfer"); return -1; }
	
	#ifdef DEBUG
		if(transferred > 0){printf("Data send : %d\n",transferred);}
	#endif

	return 0;
}

int release_kernel(){
	/**
	 * \fn int release_kernel ( void ) 
	 * \brief Libere le peripherique s'il avait deja etait pris par le noyau
	 *
	 * \param Aucun
	 * \return 0 au succes, -1 a l'echec
	 */
	#ifdef DEBUG
		printf("RELEASE KERNEL : Handle = %p\n",(void *)handle);
		printf("INTERFACES : N1 %d -- N2 %d\n",*(interfaces_list+0),*(interfaces_list+1));
	#endif
	
	int i;
	for(i=0;i<nb_interfaces;i++){	
		if(libusb_kernel_driver_active(handle,*(interfaces_list+i))){
			int status=libusb_detach_kernel_driver(handle,*(interfaces_list+i));
			if(status!=0){perror("libusb_detach_kernel_driver"); return (-1);}
		}
	}

	#ifdef DEBUG
		printf("RELEASE KERNEL : Success\n");
	#endif
	
	return 0;
}

int init_xbee_controller(void){
	/**
	 * \fn int init_xbee_controller ( void )
	 * \brief Initialise la connexion avec la cle USB. Fonction a executer avant l'envoie ou reception d'un message
	 *
	 * \param Aucun
	 * \return 0 au succes, -1 a l'echec
	 */
		
	init_context();
	int status;
	if((status=find_target()) == -1){perror("init_xbee_controller.find_target"); return -1;}
	if((status=get_interface_number()) == -1){perror("init_xbee_controller.get_interface_number"); return -1;}
	if((status=release_kernel()) == -1){perror("init_xbee_controller.release_kernel"); return -1;}
	if((status=claim_all_interfaces()) == -1){perror("init_xbee_controller.claim_all_interfaces"); return -1;}
	return 0;
}

int close_xbee_controller(void){
	/**
	 * \fn int close_xbee_controller ( void )
	 * \brief Ferme la connexion avec la cle USB. Fonction a executer a la fin du programme
	 *
	 * \param Aucun
	 * \return 0 au suecces, -1 a l'echec
	 */
	int status;
	if((status=release_all_interfaces())!=0){perror("close_xbee_controller.release_all_interfaces"); return -1;}
	libusb_close(handle);
	libusb_exit(context);
	return 0;
}
