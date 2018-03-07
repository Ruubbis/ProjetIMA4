#!/bin/bash

exec libusb/socketsServer &
java -classpath client/ Client
