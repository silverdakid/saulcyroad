#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "statique.h"

Statique* createStatique(int coordX,TypeStatique type,Statique * next){
    Statique* newStat = malloc(sizeof(Statique));

    newStat->coordX= coordX;
    newStat->type=type;
    newStat->next=next;

    return newStat;
}

int getStatiqueCoordX(Statique * statique){
    if(statique == NULL) return -1;
    return  statique->coordX ;
}

TypeStatique getStatiqueType(Statique * statique){
    if(statique == NULL) return ARBRE;
    return statique->type;
}

Statique* getStatiqueNext(Statique * statique){
    if(statique == NULL) return NULL;
    return statique->next;
}

Statique* addStatique(Statique * statiques, Statique * newStatique) {
    if (newStatique == NULL) return statiques;
    if (statiques == NULL) return newStatique;

    if (getStatiqueCoordX(newStatique) < getStatiqueCoordX(statiques)) {
        newStatique->next = statiques;
        return newStatique;
    }

    Statique* current = statiques;
    while (current->next != NULL && getStatiqueCoordX(newStatique) > getStatiqueCoordX(current->next)) {
        current = current->next;
    }

    newStatique->next = current->next;
    current->next = newStatique;
    return statiques;
}

Statique* removeStatiqueAtCoord(Statique * statique, int coordX){ 
    if(statique == NULL) return NULL;
    Statique *currentStatique=statique;
    Statique *previousStatique = NULL;

    while (currentStatique != NULL && getStatiqueCoordX(currentStatique) != coordX) {
        if(getStatiqueCoordX(currentStatique) > coordX ) return statique;
        previousStatique = currentStatique;
        currentStatique = currentStatique->next;
    }
            
    if (currentStatique != NULL) {
        Statique *toDelete = currentStatique;
        currentStatique = currentStatique->next;
        destroyStatique(toDelete);
        if (previousStatique == NULL) return currentStatique; 
        else previousStatique->next = currentStatique;
    }
    return statique;
}

Statique* forEachStatique(Statique * statiques, void (*f)(Statique*)){
    Statique* current = statiques;

    while (current != NULL) {
        Statique* next = current->next;
        f(current);
        current = next;
    }

    return statiques;
}

bool isStatiqueAtCoord(Statique * statique, int coordX){
    if(statique == NULL) return false;
    return getStatiqueCoordX(statique) == coordX;    
}

bool isStatiqueExists(Statique * statiques, int x, bool (*f)(Statique*, int)){
    Statique* current = statiques;

    while (current != NULL) {
        Statique* buffer =current;
        current = current->next;  
        if(f(buffer,x)) return true;
    }

    return false;
}

Statique *generateStatique(TypeLigne type,int size, int score){
    int numberOfStatique= numberOfElement(type,size,score); 

    Statique *current=NULL;
    while(numberOfStatique > 0){
        int newcoordX = rand() % size;
        if(isStatiqueExists(current,newcoordX,isStatiqueAtCoord)) continue;

        current = addStatique(current,createStatique(newcoordX, generateStatiqueFromType(type),NULL));
        numberOfStatique--;
    }
    return current;
}

Statique * copyStatique(Statique *statique){ 
    if(statique == NULL) return NULL;
    return createStatique(
        getStatiqueCoordX(statique),
        getStatiqueType(statique),
        NULL
    );
}

Statique * copyStatiques(Statique* statiques){ 
    Statique * copy = copyStatique(statiques);

    Statique * current = getStatiqueNext(statiques);
    Statique * previous = copy;
    while(current != NULL){
        Statique * buffer= copyStatique(current);
        previous->next = buffer;
        previous = buffer;
        current = getStatiqueNext(current);
    }
    return copy;
}

void destroyStatique(Statique *statique){
    if(statique == NULL) return;
    free(statique);
}

void destroyAllStatique(Statique *statique){
    forEachStatique(statique,destroyStatique);
}

char statiqueToString(Statique *statique){
    if(statique == NULL) return '?';
    return typeStatiqueToString(getStatiqueType(statique));
}



