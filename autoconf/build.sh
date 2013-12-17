#!/bin/sh

echo "Build script started"

cd /daisypi/sense/sht11
make clean all
cd /daisypi/sense/tsl235r
make clean all