#!/bin/bash

usage() {
	echo "Usage: $0 -f sourceCfile.c"
	echo "       $0 -h shows this usage"
	echo "       $0 reads the C file 'sourceCfile.c'and types in the screen the prototype functions"
	echo "		to be included in 'sourceCfile.h'"
	echo "		cproto is required to run this script"
}

checkParams() {
	if [ "$FILE" == "" ]; then
		usage
		exit 1
	fi
}

fileNotFound() {
	echo "ERROR: Unable to read file" 
	exit 2
}

####################
# ----- MAIN ----- # 
####################
while getopts f:h option; do
	case "${option}" 
	in
		f) FILE=${OPTARG};;
		h) usage; exit;;
		*) echo "ERROR: Invalid option";usage; exit 3;;

	esac
done

checkParams

[ -f $FILE -a -r $FILE ] || fileNotFound

includes=`pkg-config --cflags-only-I gtk+-3.0 gmodule-2.0 glib-2.0 libxml-2.0 libpulse-simple libpulse`
#echo $includes

cproto $includes -I./include $FILE
