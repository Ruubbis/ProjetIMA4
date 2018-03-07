#!/bin/bash

dfu-programmer atmega16u2 erase
dfu-programmer atmega16u2 flash XbeeController.hex
dfu-programmer atmega16u2 reset
