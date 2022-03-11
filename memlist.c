/*
 * SO LabAssignment3
 *
 * AUTHOR 1: Guillermo Fernández Sánchez | login: guillermo.fernandezs
 * AUTHOR 2: Javier Fernández Rozas      | login: j.frozas
 *
*/

#include "memlist.h"

#define LNULL NULL

void createMemList (memList *l){
/*creates a list*/

    *l = LNULL;

}

bool isEmptyMemList(memList l){
/*checks if the list is empty*/

    return (l == LNULL);
}

memPos memFirst(memList l){
/*returns the first position of the list*/

    return l;

}


memPos memNext(memPos p, memList l){
/*returns the node that is on the next position to the one inserted*/

    return p->next;

}

bool createMemNode(memPos * p){
/*auxiliary op to insertItem*/

    *p = malloc(sizeof(struct tNodeMem));

    return *p != LNULL;

}


bool insertMemItem(tItemMem d, memList* l){
/*inserts an item into the list*/


    memPos n, r;

    /*Exceptional case: no memory*/
    if ( !createMemNode(&n) )
        return false;
    else{
        /* There is enough memory. n = new node*/
        n->data = d;
        n->next = NULL;


        if ( *l == LNULL ){
            /* l is an empty list*/

            *l = n;

        } else { //Insert at the end

            for ( r = *l ; r->next != LNULL ; r = r->next ) ;

            r->next = n;
        }
        return true;
    }
}


void clearMemList(memList *l){
/*removes every element from the list*/

    memPos p;
    while(*l!=NULL){
        p=*l;
        *l = (*l)->next;
        free(p);
    }
}

tItemMem getMemItem(memPos p, memList l){
/*returns the item in a position given*/

    return p ->data;
}

void deleteAtMemPosition(memPos p, memList *l){
/* Deletes a position of the list */

    memPos q;

    if ( p == *l )
        //delete first element
        *l = p->next;
    else if ( p->next == LNULL ){
        //delete last element
        for (q = *l ; q->next != p ; q = q->next);

        q->next = LNULL;
    }
    else{
        //delete intermediate position
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q;
    }

    free(p);

}