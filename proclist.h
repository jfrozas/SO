/*
 * SO LabAssignment3
 *
 * AUTHOR 1: Guillermo Fernández Sánchez | login: guillermo.fernandezs
 * AUTHOR 2: Javier Fernández Rozas      | login: j.frozas
 *
*/

#ifndef PRACTSO_PROCLIST_H
#define PRACTSO_PROCLIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct tItemProc {
    pid_t pid;
    uid_t user;
    int signal; //state: 1-RUNNING 2-STOPPED, 3-TERMINATED, 4-TERMINATEDSIGNAL
    time_t exectime;
    char name[1024];
    char state[16];

} tItemProc;

typedef struct tNodeProc * procPos;
struct tNodeProc
{
    tItemProc data;
    procPos next;
};
typedef procPos procList;

/* function prototypes*/
void createProcList (procList *);
bool insertProcItem(tItemProc, procList*);
procPos procFirst(procList);
procPos procNext(procPos, procList);
tItemProc getProcItem(procPos, procList);
bool isEmptyProcList(procList);
void clearProcList(procList *);
void deleteAtProcPosition(procPos, procList *);
void updateItem(tItemProc, procPos, procList*);


#endif
