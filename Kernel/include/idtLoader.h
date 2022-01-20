#ifndef _IDTLOADER_H_
#define _IDTLOADER_H_

void load_idt();
extern uint64_t _sysCallHandler();

#endif