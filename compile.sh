#!/bin/bash
 
sudo docker run -v ${PWD}:/root -ti -w /root agodio/itba-so:1.0 ${1} make clean
sudo docker run -v ${PWD}:/root -ti -w /root agodio/itba-so:1.0 ${1} make all