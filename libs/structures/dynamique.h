#ifndef DYNAMQUE
#define DYNAMQUE

#include <stdbool.h>
#include "../types/types.h"

// -------- CLASS POUR LES OBJECTS QUI SE DÉPLACENT ( en liste chaîné ) -------- //

typedef struct _Dynamique{ 
    int coordX;
    int longueur;
    TypeDynamique type;
    int  ticBeforeMove;
    struct _Dynamique *next;
} Dynamique; 

// =============== Fonctions =============== // 

// -> Creation  <- //
Dynamique* createDynamique(int coordX, int longueur,int ticBeforeMove, TypeDynamique type,Dynamique * next);

// -> Getter  <- //
int getDynamiqueCoordX(Dynamique * dynamique);
int getDynamiqueLongueur(Dynamique * dynamique);
int getDynamiqueTicBeforeMove(Dynamique * dynamique); 
TypeDynamique getDynamiqueType(Dynamique * dynamique);
Dynamique* getDynamiqueNext(Dynamique * dynamique);

// -> Ajout  <- //
Dynamique* addDynamique(Dynamique * dynamiques,Dynamique * newDynamique);

// -> Prog Fonctionnelle  <- //
Dynamique* forEachDynamique(Dynamique * dynamiques, void (*f)(Dynamique*));
bool isDynamiqueExists(Dynamique * dynamiques, bool (*f)(Dynamique*));

// -> Conditions  <- //
bool isDynamiqueOutsideLigne(Dynamique * dynamique, Direction direction,int limit);
bool isDynamiqueAtCoord(Dynamique * dynamique, Direction direction, int coordX);
bool isOneDynamiqueAtCoord(Dynamique * dynamique, Direction direction, int coordX);
bool isOneDynamiqueAtTheseCoord(Dynamique * dynamiques, Direction direction, int coordX,int size);

// -> Generation / Valeur par type  <- 
int generateSizeOfDynamique(TypeDynamique type);
bool isANewDynamiqueOnLigne(TypeLigne type, Dynamique* dynamiques);
Dynamique * generateDynamique(TypeLigne type,int size, int score, Direction direction);

// -> Mouvement  <- // 
void moveDynamique(Dynamique * dynamique, Direction direction, int nbTimes);  
Dynamique * moveAllDynamique(Dynamique * dynamiques, Direction direction, int limit, int nbTimes); 

// -> toString  <- //
char dynamiqueToString(Dynamique *dynamique, Direction direction);

// -> copy  <- //
Dynamique * copyDynamique(Dynamique *dynamique);
Dynamique * copyDynamiques(Dynamique* dynamiques);

// -> Destruction  <- //
void destroyDynamique(Dynamique *Dynamique);
void destroyAllDynamique(Dynamique *Dynamique);
#endif
