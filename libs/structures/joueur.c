#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "joueur.h"
#include "ligne.h"

Joueur* createJoueur(int coordY,int coordX ,int limites,int backwards , TypeJoueur type , bool estEnVie, bool isAnIA ){
    Joueur* newJoueur = malloc(sizeof(Joueur));

    newJoueur->coordY= coordY;
    newJoueur->coordX= coordX;
    newJoueur->limites= limites;
    newJoueur-> backwards = backwards;
    newJoueur->estEnVie=estEnVie;
    newJoueur->isAnIA=isAnIA;
    newJoueur->type= type;

    return newJoueur;
}

Joueur* initialiseJoueur( TypeJoueur type , bool isAnIA ){
    return createJoueur(STARTING_Y,STARTING_X(LIMIT_JOUEUR),LIMIT_JOUEUR,0,type,true,isAnIA);
}


int getJoueurCoordX(Joueur * joueur){
    if(joueur == NULL) return -1;
    return joueur->coordX;
}

int getJoueurCoordY(Joueur * joueur){
    if(joueur == NULL) return -1;
    return joueur->coordY;
}

int getJoueurLimites(Joueur * joueur){
    if(joueur == NULL) return -1;
    return joueur->limites;
}

int getJoueurBackwards(Joueur * joueur){
    if(joueur == NULL) return -1;
    return joueur->backwards;
}

TypeJoueur getJoueurType(Joueur * joueur){
    if(joueur == NULL) return POULET;
    return joueur->type;
}

bool isAlive(Joueur * joueur){
    if(joueur == NULL) return false;
    return joueur->estEnVie;
}

bool isAnIA(Joueur * joueur){
    if(joueur == NULL) return false;
    return joueur->isAnIA;
}

bool willBeInsideLimites(int newCoordX, int joueurLimites){
    return LIMITE_AFFICHAGE <= newCoordX && newCoordX < joueurLimites+LIMITE_AFFICHAGE;
}

void reduceBackward(Joueur * joueur){
    if(joueur == NULL) return ;
    int back = getJoueurBackwards(joueur);
    if(back == 0) return ;
    joueur->backwards = back-1;
}

bool addBackward(Joueur * joueur){
    if(joueur == NULL) return false;
    joueur->backwards = getJoueurBackwards(joueur) +1;
    if( getJoueurBackwards(joueur) >= LIMIT_BACKWARD) {
        killJoueur(joueur);
        return true;
    }
    return false;
}

void setJoueurCoordX(Joueur * joueur,int coordX ){
    if(joueur == NULL) return ;
    joueur->coordX=coordX;
}

void setJoueurCoordY(Joueur * joueur,int coordY ){
    if(joueur == NULL) return ;
    joueur->coordY=coordY;
}


void killJoueur(Joueur * joueur){
    if(joueur == NULL) return;
    joueur->estEnVie=false;
}

void moveJoueur(Joueur *joueur, Direction direction,bool couldGoOutsideLimite){ 
    if(joueur == NULL) return;
    switch (direction)
    {
    case GAUCHE : {
        bool willBeInside = willBeInsideLimites(getJoueurCoordX(joueur)-1, getJoueurLimites(joueur));
        if(willBeInside || couldGoOutsideLimite) setJoueurCoordX(joueur,getJoueurCoordX(joueur)-1);
        if(!willBeInside && couldGoOutsideLimite) killJoueur(joueur);
        break;
    }
    case DROITE: {
        bool willBeInside = willBeInsideLimites(getJoueurCoordX(joueur)+1, getJoueurLimites(joueur));
        if(willBeInside || couldGoOutsideLimite) setJoueurCoordX(joueur,getJoueurCoordX(joueur)+1);
        if(!willBeInside && couldGoOutsideLimite) killJoueur(joueur);
        break;
    }
    case HAUT: {
        setJoueurCoordY(joueur,getJoueurCoordY(joueur)+1);
        reduceBackward(joueur);
        break;
    }
    case BAS: {
        setJoueurCoordY(joueur,getJoueurCoordY(joueur)-1);
        addBackward(joueur);
        break;
    }
    default:
        break;
    }
}

char joueurToString(Joueur *joueur){
    if(joueur == NULL) return '?';
    return typeJoueurToString(getJoueurType(joueur));
}

void destroyJoueur(Joueur *joueur){
    if(joueur == NULL) return;
    free(joueur);
}

Joueur * copyJoueur(Joueur* joueur){ 
    if(joueur == NULL) return NULL;
    return createJoueur(
        getJoueurCoordY(joueur),
        getJoueurCoordX(joueur),
        getJoueurLimites(joueur),
        getJoueurBackwards(joueur),
        getJoueurType(joueur),
        isAlive(joueur),
        isAnIA(joueur)
    );
}
