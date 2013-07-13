#!/bin/sh

NEW_SERVER_LOCATION="$1"
OLD_SERVER_LOCATION="$2"

#Stop old

#Update
mv -r ${NEW_SERVER_LOCATION} ${OLD_SERVER_LOCATION}

#Start new 
