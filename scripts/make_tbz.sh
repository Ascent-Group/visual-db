#!/bin/sh

SCRIPT_FILE=`basename $0`

if [ ! -f $SCRIPT_FILE ]; then
    echo "Has to be run from visual-db/scripts";
fi

#pkg_create
