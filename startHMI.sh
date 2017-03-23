#!/usr/bin/env bash

# Es darf nur eine Instanz laufen
pgrep hmi > /dev/null && exit

while true
do
	authbind ./hmi
	rc=$?
	if [ $rc -eq 50 ]
	then
		#Schliessen
		exit
	elif [ $rc -eq 51 ]
	then
		reboot
		exit
	elif [ $rc -eq 52 ]
	then
		poweroff
		exit
	fi
done
