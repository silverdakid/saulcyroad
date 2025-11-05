#ifndef PLATEAU
#define PLATEAU

#include <stdbool.h>

#include "ligne.h"
#include "../types/types.h"

// -------- CLASS POUR L'IA ET L'ANTICIPATION DES PROCHAINES PLATEAUX -------- //

typedef struct _Plateau{ 
    Ligne *lignes;
    bool haveDynamiquesMoved;
    struct _Plateau * next;
} Plateau; 


//=============== Fonctions ===============// 

// -> Creation <- //
Plateau* createPlateau(Ligne *lignes,bool haveDynamiquesMoved,Plateau * next); 

// -> Getter <- //
Ligne *getPlateauLigne(Plateau * plateau);
bool haveDynamiquesMoves(Plateau * plateau);
Plateau* getPlateauNext(Plateau * plateau);

// -> Ajout <- //
Plateau* addPlateau(Plateau * plateaux,Plateau * newPlateau); 

// -> Suppresion du premier <- //
Plateau* removeFirstPlateau(Plateau * plateaux); 

// -> Generation <- //
Plateau* generatePlateaux(Ligne *currentLines,int profondeur,double bufferClock, Joueur *joueur);

Plateau* forEachPlateau(Plateau * plateaux, void (*f)(Plateau*));

// -> Destruction <- //
void destroyPlateau(Plateau *plateau);
void destroyAllPlateau(Plateau *plateaux); 

#endif
