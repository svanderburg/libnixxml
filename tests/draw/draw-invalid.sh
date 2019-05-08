#!/bin/sh

./draw xml invalid.xml

if [ $? -eq 1 ]
then
    exit 0
else
    exit 1
fi
