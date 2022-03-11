/*
 * SO LabAssignment0
 *
 * AUTHOR 1: Guillermo Fernández Sánchez | login: guillermo.fernandezs
 * AUTHOR 2: Javier Fernández Rozas      | login: j.frozas
 *
*/

#ifndef SO_LIST_H
#define SO_LIST_H
#define LNULL NULL

#define MAXCMDLINE 1024

#include <stdbool.h>



typedef char line[MAXCMDLINE];

typedef struct tItemL {
    line cmdline;
} tItemL;

typedef struct tNode * LPos;
struct tNode
{
    tItemL data;
    LPos next;
};
typedef LPos tList;

/* function prototypes*/
void createList (tList *);
bool insertItem(tItemL, tList*);
LPos first(tList);
LPos next(LPos, tList);
tItemL getItem(LPos, tList);
bool isEmptyList(tList);
void clearList(tList *);


#endif
