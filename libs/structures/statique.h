#ifndef STATIQUE
#define STATIQUE

#include <stdbool.h>
#include "../types/types.h"

// -------- CLASS POUR LES OBJECTS QUI NE SE DÉPLACENT PAS ( en liste chaîné ) -------- //

typedef struct _Statique{ 
    int coordX;
    TypeStatique type;
    struct _Statique *next;
} Statique; 


//=============== Fonctions ===============// 

// -> Creation <- //
Statique* createStatique(int coordX,TypeStatique type,Statique * next); 

// -> Getter <- //
int getStatiqueCoordX(Statique * statique);
TypeStatique getStatiqueType(Statique * statique);
Statique* getStatiqueNext(Statique * statique);

// -> Ajout et suppression<- //
Statique* addStatique(Statique * statiques,Statique * newStatique); 
Statique* removeStatiqueAtCoord(Statique * statique, int coordX);

// -> Programmation fonctionnel <- //
Statique* forEachStatique(Statique * statiques, void (*f)(Statique*));
bool isStatiqueExists(Statique * statiques, int x, bool (*f)(Statique*, int)); 

// -> where <- //
bool isStatiqueAtCoord(Statique * dynamique, int coordX);

// -> generation <- //
Statique *generateStatique(TypeLigne type,int size, int score);

// -> toString <- //
char statiqueToString(Statique *statique);

// -> copy <- //
Statique * copyStatique(Statique *statique);
Statique * copyStatiques(Statique* statiques);

// -> Destruction <- //
void destroyStatique(Statique *statique);
void destroyAllStatique(Statique *statique); 

#endif
