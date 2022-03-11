/*
 * SO LabAssignment2
 *
 * AUTHOR 1: Guillermo Fernández Sánchez | login: guillermo.fernandezs
 * AUTHOR 2: Javier Fernández Rozas      | login: j.frozas
 *
*/

#include "list.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LNULL NULL


void createList(tList *l){
/*creates a list*/

    *l = LNULL;

}

bool isEmptyList(tList l){
/*checks if the list is empty*/

    return (l == LNULL);
}

LPos first(tList l){
/*returns the first position of the list*/

    return l;

}


LPos next(LPos p, tList l){
/*returns the node that is on the next position to the one inserted*/

    return p->next;

}

bool createNode(LPos * p){
/*auxiliary op to insertItem*/

    *p = malloc(sizeof(struct tNode));

    return *p != LNULL;

}


bool insertItem(tItemL d, tList *l){
/*inserts an item into the list*/


    LPos n, r;

    /*Exceptional case: no memory*/
    if ( !createNode(&n) )
        return false;
    else{
    /* There is enough memory. n = new node*/
        n->data = d;
        n->next = NULL;


        if ( *l == LNULL ){ 
        /* l is an empty list*/

            *l = n;

        }

        else /*Insert at the end */
        {
            for ( r = *l ; r->next != LNULL ; r = r->next ) ;

            r->next = n;


        }

        return true;

    }

}


void clearList(tList *l){
/*removes every element from the list*/

    LPos p;
    while(*l!=NULL){
        p=*l;
        *l = (*l)->next;
        free(p);
    }

}

tItemL getItem(LPos p, tList l){
/*returns the item in a position given*/

    return p ->data;
}

