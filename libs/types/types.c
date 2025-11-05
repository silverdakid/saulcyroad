#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "types.h"

// -------- CONVERTIR LES TYPES EN STRING -------- //

char typeStatiqueToString(TypeStatique type){
    switch(type){
        case NENUPHARE: return 'o';
        case BUISSON: return '@';
        case ARBRE: return '#';
        default: return '?';
    }
}

char typeDynamiqueToString(TypeDynamique type,Direction direction){
    switch(type){
        case VOITURE : return direction == GAUCHE ? '<' :'>';
        case CAMION: return direction == GAUCHE ? '<' : '>';
        case TRAIN: return direction == GAUCHE ? '<' : '>' ;
        case BUCHE: return direction == GAUCHE ? '<' : '>' ;
        default: return '?';
    }
}

char typeLigneToString(TypeLigne type){
    switch(type){
        case ROUTE: return '-';
        case HERBE: return '\"';
        case VOIE_FERREE: return '=';
        case FLEUVE:
        case RAPIDE: return '~';
        default: return '?';
    }
}

char typeJoueurToString(TypeJoueur type){
    switch(type){
        case POULET: return '*';
        case CANARD: return '*';
        default: return '?';
    }
}

// -------- PROBABILITÉS D'APPARITION -------- //
float * typeLigneProbability(TypeLigne type){ 
    //  ROUTE, HERBE, VOIE_FERREE, FLEUVE, RAPIDE
    static float route[]      = { 0.40, 0.20, 0.20, 0.10, 0.10 };
    static float herbe[]      = { 0.40, 0.15, 0.20, 0.15, 0.10 };
    static float voieFerree[] = { 0.30, 0.15, 0.20, 0.20, 0.15 };
    static float fleuve[]     = { 0.15, 0.15, 0.15, 0.20, 0.35 };
    static float rapide[]     = { 0.10, 0.20, 0.10, 0.25, 0.40 };

    switch (type) {
        case ROUTE: return route;
        case HERBE: return herbe;
        case VOIE_FERREE: return voieFerree;
        case FLEUVE: return fleuve;
        case RAPIDE: return rapide;
        default: return NULL;
    }
}

int numberOfElement(TypeLigne type, int size, int score) {  
    int difficulty = score / COEFDIFFICULTY ;
    bool isMaxDifficulty =  difficulty >= size; 
    switch (type) {
        case ROUTE: {
            return rand() % 10 + 4; 
        }
        case HERBE: {
            if(score==-1) return rand() % size/3 + 4;

            int min = 4;
            int max = size/2;
            int adjusted = isMaxDifficulty ? (max-min) : (difficulty)/2 ;
            if (adjusted < 0) adjusted = (max-min);
            return rand() % (max - adjusted) + (min + adjusted); 
        }
        case VOIE_FERREE: return rand() % 2;
        case FLEUVE: {
            int min = 2;
            int max = size/2;
            int adjusted = isMaxDifficulty ? (max-min):  (max-min) - (difficulty)/2 ;
            if (adjusted < 0) adjusted = 0;
            return rand() % (max - adjusted) + (min + adjusted); 
        }
        case RAPIDE: {
            return rand() % 8 + 3; 
        }
        default:
            return 0;
    }
    // switch (type) {
    //     case ROUTE: {
    //         int max = 8;// size/4;
    //         int min = 0;
    //         int adjusted = isMaxDifficulty ? (max-min) : (max-min) - (difficulty)/2 ;
    //         if (adjusted < 0) adjusted = 0;
    //         return rand() % (max - adjusted) + ( min + adjusted); 
    //     }
    //     case RAPIDE: {
    //         int min = 3;
    //         int max = 8;//size/5;
    //         int adjusted = isMaxDifficulty ? (max-min):  (max-min) -(difficulty)/2 ;
    //         if (adjusted < 0) adjusted = 0;
    //         return rand() % (max - adjusted) + (min + adjusted); 
    //     }
    //     default:
    //         return 0;
    // }
}


TypeStatique generateStatiqueFromType(TypeLigne type) {
    switch (type) {
        case HERBE: 
            return rand() % 2 ? ARBRE : BUISSON;
        case FLEUVE: return NENUPHARE;
        default: return BUISSON;
    }
} 

TypeDynamique generateDynamiqueFromType(TypeLigne type) {
    switch (type) {
        case ROUTE: 
        return rand() % 2 ? CAMION : VOITURE; 
        case VOIE_FERREE: return TRAIN;  
        case RAPIDE: return BUCHE;
        default: return VOITURE;
    }
}

Direction generateDirectionForDynamique(void){
    return rand() % 2 == 0 ? GAUCHE : DROITE;
}

int getDirectionNextCoordX(Direction direction){
    if( direction == GAUCHE) return -1;
    else if( direction == DROITE ) return 1;
    return 0;
}

int getDirectionNextCoordY(Direction direction){
    if( direction == BAS) return -1;
    else if( direction == HAUT ) return 1;
    return 0;
}

