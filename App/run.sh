#!/bin/bash
if [ -f "libusb/socketsServer" ]
then
	exec libusb/socketsServer &
	java -classpath client/ Client
else
	echo "Veuillez executer la commande : make"
fi
