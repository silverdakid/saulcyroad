#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "chemin.h"
#include "../types/types.h"

Chemin* createChemin(int coordY,int coordX, int score,Direction startingDirection, Chemin *next ){
    Chemin* newChemin = malloc(sizeof(Chemin));

    newChemin->coordY= coordY;
    newChemin->coordX= coordX;
    newChemin->score= score;
    newChemin->startingDirection= startingDirection;
    newChemin-> next = next;
    
    return newChemin;
}


int getCheminCoordX(Chemin * chemin){
    if(chemin == NULL) return -1;
    return chemin->coordX;
}

int getCheminCoordY(Chemin * chemin){
    if(chemin == NULL) return -1;
    return chemin->coordY;
}

int getCheminScore(Chemin * chemin){
    if(chemin == NULL) return -1;
    return chemin->score;
}

Direction getCheminStartingDirection(Chemin * chemin){
    if(chemin == NULL) return -1;
    return chemin->startingDirection;
}

Chemin *getCheminNext(Chemin * chemin){
    if(chemin == NULL) return NULL;
    return chemin->next;
}

bool isCheminAtCoord(int coordY, int coordX,Chemin *chemin){ 
    return coordX == getCheminCoordX(chemin) && coordY == getCheminCoordY(chemin);
}

bool isANewBestDirection(Direction currentDirection, Direction newDirection, int startX, int centerX){
    return (
        newDirection == HAUT || 
        (currentDirection == BAS && newDirection != BAS) || 
        (currentDirection == -1 && newDirection != BAS && newDirection !=-1) ||
        (currentDirection == GAUCHE && startX < centerX && newDirection == DROITE) ||
        (currentDirection == DROITE && startX > centerX && newDirection == GAUCHE));
}

void setBestDirection(Chemin * chemin,Direction newDirection, int startX, int centerX){
    if( chemin == NULL) return ;
    Direction  currentDirection = getCheminStartingDirection(chemin);
    if ( isANewBestDirection(currentDirection,newDirection,startX,centerX) ) {
        chemin->startingDirection = newDirection;
    }
}

// -> modifie aussi la meilleur position de départ pour le chemin si elle est mieux <- //
bool isCheminEndExisting(int coordY, int coordX,int newScore, Direction newStartingDirection, Chemin * chemins, int startX, int centerX){ 
    if (chemins == NULL) return false;

    Chemin* current = chemins;
    while (current->next != NULL ) {
        if(isCheminAtCoord(coordY,coordX,current)){ 
            if( newScore > getCheminScore(current)){
                current -> score = newScore;
                current -> startingDirection = newStartingDirection;
            }else if( newScore == getCheminScore(current)){
                setBestDirection(current,newStartingDirection,startX,centerX); 
            }
            return true;
        }

        current = current->next;
    }

    return false;
}

Direction getBestStartingDirection(Chemin * chemins, int startX, int centerX){
    if(chemins == NULL) return HAUT;

    Chemin* current = chemins;
    Chemin* buffer = getCheminNext(chemins);

    while( getCheminScore(current) == getCheminScore(buffer) ){
        if( isANewBestDirection(getCheminStartingDirection(current),getCheminStartingDirection(buffer),startX,centerX)) current = buffer;
        buffer = getCheminNext(buffer);
    }
    return getCheminStartingDirection(current);
}

Chemin* addChemin(Chemin *newChemin, Chemin * chemins){
    if (newChemin == NULL) return chemins;
    if (chemins == NULL) return newChemin;

    if (getCheminScore(chemins) < getCheminScore(newChemin)) {
        newChemin->next = chemins;
        return newChemin;
    }

    Chemin* current = chemins;
    while (current->next != NULL && getCheminScore(current->next) > getCheminScore(newChemin)) {
        current = current->next;
    }

    newChemin->next = current->next;
    current->next = newChemin;
    return chemins;
}

void destroyChemin(Chemin *chemin){
    if(chemin == NULL) return;
    free(chemin);
}

void destroyAllChemin(Chemin *chemin){
    Chemin *current = chemin;

    while( current != NULL){
        Chemin *buffer = current;
        current = getCheminNext(current);
        destroyChemin(buffer);
    }
    
}




