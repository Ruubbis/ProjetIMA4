#!/bin/bash
if [ -f "libusb/app" ]
then
	exec libusb/app &
	java -classpath client/ Client
else
	echo "Veuillez executer la commande : make"
fi
