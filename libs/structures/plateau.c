#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "plateau.h"
#include "ligne.h"

Plateau* createPlateau(Ligne *lignes,bool haveDynamiquesMoved,Plateau * next){
    Plateau* newPlat = malloc(sizeof(Plateau));

    newPlat->lignes= lignes;
    newPlat->haveDynamiquesMoved=haveDynamiquesMoved;
    newPlat->next=next;

    return newPlat;
}


// -> Getter <- //
Ligne* getPlateauLigne(Plateau * plateaux){
    if(plateaux == NULL) return NULL;
    return  plateaux->lignes ;
}

bool haveDynamiquesMoves(Plateau * plateaux){
    if(plateaux == NULL) return true;
    return  plateaux->haveDynamiquesMoved ;
}

Plateau* getPlateauNext(Plateau * Plateau){
    if(Plateau == NULL) return NULL;
    return  Plateau->next ;
}

// -> Ajout <- //
Plateau* addPlateau(Plateau * plateaux,Plateau * newPlateau){
    if (newPlateau == NULL) return plateaux;
    if (plateaux == NULL) return newPlateau;

    Plateau* current = plateaux;
    while (current->next != NULL ) {
        current = current->next;
    }

    newPlateau->next = current->next;
    current->next = newPlateau;
    return plateaux;
}

// -> Suppresion du premier <- //
Plateau* removeFirstPlateau(Plateau * plateaux){
    Plateau *current = getPlateauNext(plateaux);
    destroyPlateau(plateaux);
    return current;
}

// -> Generation <- //
Plateau* generatePlateaux(Ligne *currentLines,int profondeur,double bufferClock, Joueur *joueur){
    if(currentLines == NULL || joueur == NULL) return NULL;
    
    double currentClock = bufferClock;

    
    Plateau *startingPlateau = createPlateau(copyLignes(currentLines),false,NULL);
    Plateau *currentPlateau = startingPlateau;
        
    double acc = 1;
    // -> Boucle sur la profonduer 
    for(int idx=0; idx< profondeur; idx ++){ 
        // -> Ajout du temps à la clock
        currentClock += COEF_IA;

        // -> Par rapport au current plateau
        Plateau * bufferPlateau = createPlateau(copyLignes(getPlateauLigne(currentPlateau)),false,NULL);

        // -> Déplacement des trains
        moveDynamiquesOnAllLignes(getPlateauLigne(bufferPlateau),joueur,false,COEF_IA/COEF_FAST);

        // -> Déplacement des véhicules
        if( currentClock >= COEF_SLOW*acc ){ 
            moveDynamiquesOnAllLignes(getPlateauLigne(bufferPlateau),joueur,true,1);
            bufferPlateau->haveDynamiquesMoved=true;
            acc = acc+1;
        }

        //printAll(getPlateauLigne(bufferPlateau),joueur,0);
        // -> Stockage dans le current plateau et current = next
        currentPlateau->next = bufferPlateau;
        currentPlateau = currentPlateau->next;
    }
    return startingPlateau; 
}

Plateau* forEachPlateau(Plateau * plateaux, void (*f)(Plateau*)){
    Plateau* current = plateaux;

    while (current != NULL) {
        Plateau* next = current->next;
        f(current);
        current = next;
    }

    return plateaux;
}

// -> Destruction <- //
void destroyPlateau(Plateau *plateau){
    if(plateau == NULL) return;
    destroyAllLigne(getPlateauLigne(plateau));
    free(plateau);
}

void destroyAllPlateau(Plateau *plateaux){
    forEachPlateau(plateaux,destroyPlateau);
}


