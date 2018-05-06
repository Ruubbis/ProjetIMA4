#include "libXbeeController.h"


int configValue;
int * interfaces_list = NULL;
int nb_interfaces;
libusb_device_handle * handle;
int sockfd;
libusb_context *context;
int endpoint_in, endpoint_out;

int init_context(){
	int status=libusb_init(&context);
	if(status!=0){perror("libusb_init"); return -1;}
	return 0;
}


int find_target(){
	libusb_device ** list;
	ssize_t count = libusb_get_device_list(context,&list);
	if(count<0) {perror("libusb_get_device_list"); exit(-1);}
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
			if(success!=0){perror("libusb_open"); return -1;}
			return 0;
		}
	}
	#ifdef DEBUG
		printf("target %x:%x not found\n",ID_VENDOR,ID_PRODUCT);
	#endif
	return -1;
}

int get_interface_number(){
//Recupere le numero de l'interface utilise et le numero de la configuration du peripherique.
//Retourne 0 au succes, sinon -1
	int k;
	libusb_device * target_device = libusb_get_device(handle);
	struct libusb_config_descriptor * config;
	int success = libusb_get_config_descriptor(target_device,0,&config);

	if(success!=0){perror("libusb_get_config_descriptor"); return -1;}
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
	#ifdef DEBUG
		printf("CLAIM INTERFACE : Handle = %p -- interfaceNumber = %d\n",(void *)handle,interfaceNumber);
	#endif
	int status=libusb_claim_interface(handle,interfaceNumber);
	if(status!=0){perror("libusb_claim_interface"); return -1;}
	return 0;
}

int release_interface(int interfaceNumber){
	#ifdef DEBUG
		printf("RELEASE INTERFACE : Handle = %p -- interfaceNumber = %d\n",(void *)handle,interfaceNumber);
	#endif
	int status = libusb_release_interface(handle,interfaceNumber);
	if(status!=0){perror("libusb_release_interface"); return -1;}
	return 0;
}

int claim_all_interfaces(){
	#ifdef DEBUG
		printf("CLAIM ALL INTERFACE : Handle = %p -- ConfigValue = %d\n",(void *)handle, configValue);
	#endif
	int i;
	int status = libusb_set_configuration(handle,configValue);
	if(status!=0){perror("libusb_set_configuration"); return -1;}
	
	for(i=0;i<nb_interfaces;i++){
		claim_interface(*interfaces_list+i);
	}
	return 0;
}

int release_all_interfaces(){
	
	#ifdef DEBUG
		printf("RELEASE ALL INTERFACE : Handle = %p \n",(void *)handle);
	#endif
	
	int i;
	for(i=0;i<nb_interfaces;i++){
		int status = release_interface((*interfaces_list+i));
		if(status!=0){perror("release_all_interfaces"); return -1;}
	}	
	free(interfaces_list);
	return 0;
}

int read_endpoint(){

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

int write_endpoint(int led_state){
	
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

int release_kernel(){ 
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

int init_xbee_controller(void){
	init_context();
	int status = find_target();
	if(status == -1){perror("find_target"); return -1;}
	
	get_interface_number();
	release_kernel();

	claim_all_interfaces();
	return 1;
}

int close_xbee_controller(void){
	release_all_interfaces();
	libusb_close(handle);
	libusb_exit(context);
	return 1;
}
