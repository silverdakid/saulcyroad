
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dynamique.h"
#include "../types/types.h"


Dynamique* createDynamique(int coordX, int longueur,int ticBeforeMove, TypeDynamique type,Dynamique * next){
    Dynamique* newDyn = malloc(sizeof(Dynamique));

    newDyn->coordX= coordX;
    newDyn->longueur= longueur;
    newDyn->ticBeforeMove= ticBeforeMove;
    newDyn->type=type;
    newDyn->next=next;

    return newDyn;
}

int getDynamiqueCoordX(Dynamique * dynamique){
    if(dynamique == NULL) return -1;
    return  dynamique->coordX ;
}

int getDynamiqueLongueur(Dynamique * dynamique){
    if(dynamique == NULL) return 1;
    return  dynamique->longueur ;
}

int getDynamiqueTicBeforeMove(Dynamique * dynamique){ 
    if(dynamique == NULL) return 0;
    return  dynamique->ticBeforeMove ;
}

TypeDynamique getDynamiqueType(Dynamique * dynamique){
    if(dynamique == NULL) return VOITURE;
    return dynamique->type;
}

Dynamique* getDynamiqueNext(Dynamique * dynamique){
    if(dynamique == NULL) return NULL;
    return dynamique->next;
}

Dynamique* addDynamique(Dynamique* dynamiques, Dynamique* newDynamique) {
    if (newDynamique == NULL) return dynamiques;
    if (dynamiques == NULL) return newDynamique;

    if (getDynamiqueCoordX(newDynamique) < getDynamiqueCoordX(dynamiques)) {
        newDynamique->next = dynamiques;
        return newDynamique;
    }

    Dynamique* current = dynamiques;
    while (current->next != NULL && getDynamiqueCoordX(newDynamique) >= getDynamiqueCoordX(current->next)) {
        current = current->next;
    }

    newDynamique->next = current->next;
    current->next = newDynamique;

    return dynamiques;
}

void moveDynamique(Dynamique * dynamique, Direction direction, int nbTimes){
    if( dynamique == NULL) return;
    dynamique->coordX = direction == GAUCHE ?
        getDynamiqueCoordX(dynamique) - nbTimes
        :
        getDynamiqueCoordX(dynamique) + nbTimes;
}

bool isDynamiqueOutsideLigne(Dynamique * dynamique, Direction direction,int limit){
    if(dynamique == NULL) return false;
    return direction == GAUCHE ?
        (getDynamiqueCoordX(dynamique)+getDynamiqueLongueur(dynamique)-1 < 0)
        :
        (getDynamiqueCoordX(dynamique)-getDynamiqueLongueur(dynamique)+1 >= limit );
    
}

bool isDynamiqueAtCoord(Dynamique * dynamique, Direction direction, int coordX){
    if(dynamique == NULL) return false;
    return direction == GAUCHE ?
        (getDynamiqueCoordX(dynamique) <= coordX && coordX < getDynamiqueCoordX(dynamique)+getDynamiqueLongueur(dynamique)) 
        :
        (getDynamiqueCoordX(dynamique)-getDynamiqueLongueur(dynamique) < coordX && coordX <= getDynamiqueCoordX(dynamique));
}

// -> Vérifier si un dynamique se trouve sur la coordonnée X
bool isOneDynamiqueAtCoord(Dynamique * dynamiques, Direction direction, int coordX){
    Dynamique* current = dynamiques;

    while (current != NULL) {
        Dynamique* buffer =current;
        current = current->next;  
        if(isDynamiqueAtCoord(buffer,direction,coordX)) return true;
    }

    return false;
}

// -> Pour le génération d'élément vérifie si la place est libre sur la longueur
bool isOneDynamiqueAtTheseCoord(Dynamique * dynamiques, Direction direction, int coordX,int size){
    Dynamique* current = dynamiques;

    while (current != NULL) {
        Dynamique* buffer =current;
        current = current->next;
        for(int idx=0;idx < size ;idx++){
            if(isDynamiqueAtCoord(buffer,direction,direction == GAUCHE ? coordX + idx : coordX - idx)) return true;
        }
    }

    return false;
}

Dynamique* forEachDynamique(Dynamique * dynamiques, void (*f)(Dynamique*)){
    Dynamique* current = dynamiques;

    while (current != NULL) {
        Dynamique* next = current->next;
        f(current);
        current = next;
    }

    return dynamiques;
}

bool isDynamiqueExists(Dynamique * dynamiques, bool (*f)(Dynamique*)){
    Dynamique* current = dynamiques;

    while (current != NULL) {
        Dynamique* buffer =current;
        current = current->next;  
        if(f(buffer)) return true;
    }

    return false;
}

Dynamique * copyDynamique(Dynamique *dynamique){ 
    if(dynamique == NULL) return NULL;
    return createDynamique(
        getDynamiqueCoordX(dynamique),
        getDynamiqueLongueur(dynamique),
        getDynamiqueTicBeforeMove(dynamique),
        getDynamiqueType(dynamique),
        NULL
    );
}

Dynamique * copyDynamiques(Dynamique* dynamiques){ 
    Dynamique * copy = copyDynamique(dynamiques);

    Dynamique * current = getDynamiqueNext(dynamiques);
    Dynamique * previous = copy;
    while(current != NULL){
        Dynamique * buffer= copyDynamique(current);
        previous->next = buffer;
        previous = buffer;
        current = getDynamiqueNext(current);
    }
    return copy;
}

void destroyDynamique(Dynamique *dynamique){
    if(dynamique == NULL) return;
    free(dynamique);
}

void destroyAllDynamique(Dynamique *dynamique){
    forEachDynamique(dynamique,destroyDynamique);
}

Dynamique * moveAllDynamique(Dynamique * dynamiques, Direction direction, int limit, int nbTimes) {
    if (dynamiques == NULL) return NULL;

    Dynamique *current = dynamiques;
    Dynamique *previous = NULL;

    while (current != NULL) { 
        Dynamique *next = current->next;  
        int tic = getDynamiqueTicBeforeMove(current); 
        if( tic > 0 ){
            current->ticBeforeMove = tic - 1;
            current = next;
            continue;
        }

        moveDynamique(current, direction, nbTimes); 


        if (isDynamiqueOutsideLigne(current, direction, limit)) {
            if (previous == NULL) {
                dynamiques = next;
            } else {
                previous->next = next;
            }
            destroyDynamique(current); 
        } else {
            previous = current; 
        }
        current = next;
    }

    return dynamiques;
}

int generateSizeOfDynamique(TypeDynamique type){
    switch(type){
        case VOITURE : return  rand() % 2 + 1;
        case CAMION: return  rand() % 2 + 2 ;
        case TRAIN: return  rand() % 10 + 15;
        case BUCHE: return  rand() % 2 + 2 ;
        default: return 1;
    }
   
}

bool isANewDynamiqueOnLigne(TypeLigne type, Dynamique* dynamiques){ 
    switch (type) {
        case ROUTE: return  (rand() % 100) < 30;  //50% 
        case VOIE_FERREE:   
            if(dynamiques != NULL) return false;
            return  (rand() % 100) < 15;   //20% 
        case RAPIDE:return  (rand() % 100) < 35;  //75%  
        default: return 0;
    }
}

Dynamique * generateDynamique(TypeLigne type,int size, int score, Direction direction){ 
    int numberOfDynamique= numberOfElement(type,size,score);

    Dynamique *current=NULL;
    while(numberOfDynamique > 0){   

        
        int newcoordX = direction == GAUCHE  ? rand() % (size + (size/2))  : rand() % (size + (size/2)) - (size/2) ;
        
        
        TypeDynamique typeDyn = generateDynamiqueFromType(type);
        int dynamiqueSize = generateSizeOfDynamique(typeDyn);

        int attemp = 1;
        while(isOneDynamiqueAtTheseCoord(current,direction,newcoordX,dynamiqueSize)){ 
            if(attemp == size){
                return current;
            }
            newcoordX = (newcoordX + 1) % size;
            attemp++;
        };


        current = addDynamique(current,createDynamique(newcoordX,dynamiqueSize,0,typeDyn,NULL));
        numberOfDynamique--;
    }

    return current;
}


char dynamiqueToString(Dynamique *dynamique, Direction direction){
    return typeDynamiqueToString(getDynamiqueType(dynamique),direction);
}

