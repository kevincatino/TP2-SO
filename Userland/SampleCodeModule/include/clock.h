#ifndef _CLOCK_H_
#define _CLOCK_H_

#define YEAR 0x09
#define MOUNTH 0x08
#define DAY 0x07
#define HOURS 0x04
#define MINUTES 0x02
#define SECONDS 0x00
#define UTC 3

extern int sys_get_time(int mode);

unsigned int getFormat(unsigned int num);
unsigned int time(int mode);
void timeGame();


#endif