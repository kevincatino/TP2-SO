#!/bin/bash
 
if [ $# -ge 1 ] 
then 
    sudo docker run -v ${PWD}:/root -ti -w /root agodio/itba-so:1.0 make clean
    sudo docker run -v ${PWD}:/root -ti -w /root agodio/itba-so:1.0 make MM=${1}
else 
    sudo docker run -v ${PWD}:/root -ti -w /root agodio/itba-so:1.0 make clean
    sudo docker run -v ${PWD}:/root -ti -w /root agodio/itba-so:1.0 make all 
fi
    
