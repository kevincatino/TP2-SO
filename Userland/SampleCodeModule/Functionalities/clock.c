#include <clock.h>
#include <libc.h>

unsigned int getTimeFormat(unsigned int num) {
    int dec = num & 240;
    dec = dec >> 4;
    int units = num & 15;
    return dec * 10 + units;
}

unsigned int time(int mode) {
    unsigned int time = sys_get_time(mode);
    if(mode == HOURS) {
        int actualHour = getTimeFormat(time);
        if(actualHour == 0) {
            return 21;
        } else if(actualHour == 1) {
            return 22;
        } else if(actualHour == 2) {
            return 23;
        } 
        return actualHour - UTC;
    }
    return getTimeFormat(time);
}


void timeGame() {  
    printInScreenPosition("Screen 1", (80*2*6) + 64);
    printInScreenPosition("          Time: ", (80*2*5));
    char auxHour[3];
    char auxMinutes[3];
    char auxSeconds[3];

    intToString(time(HOURS), auxHour);
    intToString(time(MINUTES), auxMinutes);
    intToString(time(SECONDS), auxSeconds);

    printInScreenPosition(auxHour, (80*2*5) + (16*2));
    printInScreenPosition(":", (80*2*5) + (18*2));
    printInScreenPosition(auxMinutes, (80*2*5) + (19*2));
    printInScreenPosition(":", (80*2*5) + (21*2));
    printInScreenPosition(auxSeconds, (80*2*5) + (22*2));
}