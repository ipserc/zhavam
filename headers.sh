#!/bin/bash

includes=`pkg-config --cflags-only-I gtk+-3.0 gmodule-2.0 glib-2.0 libxml-2.0 libpulse-simple libpulse`
#echo $includes

cproto $includes -I./include $1
