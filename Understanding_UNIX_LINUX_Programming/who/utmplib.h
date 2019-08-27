#ifndef UTMPLIB
#define UTMPLIB
extern int utmp_open(char * filename);
extern struct  utmp* utmp_next();
extern int utmp_reload();
extern void utmp_close();
#endif 

