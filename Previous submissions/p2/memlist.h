/*
 * SO LabAssignment2
 *
 * AUTHOR 1: Guillermo Fernández Sánchez | login: guillermo.fernandezs
 * AUTHOR 2: Javier Fernández Rozas      | login: j.frozas
 *
*/

#ifndef SOPRACT_MEMLIST_H
#define SOPRACT_MEMLIST_H


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#define MAXFILELINE 256

typedef char name[MAXFILELINE];

typedef struct memData{
    key_t key;
    int fd;
    name fileName;
}memData;

typedef struct tItemMem {
    void *address;
    size_t size;
    memData data;
    time_t time;
    char type; //'1':malloc - '2':mmap - '3':shared
} tItemMem;

typedef struct tNodeMem * memPos;
struct tNodeMem
{
    tItemMem data;
    memPos next;
};
typedef memPos memList;

/* function prototypes*/
void createMemList (memList *);
bool insertMemItem(tItemMem, memList*);
memPos memFirst(memList);
memPos memNext(memPos, memList);
tItemMem getMemItem(memPos, memList);
bool isEmptyMemList(memList);
void deleteAtMemPosition(memPos, memList *);


#endif