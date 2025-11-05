#ifndef CHEMIN
#define CHEMIN

#include <stdbool.h>

#include "../types/types.h"


// -------- CLASS POUR LES CHEMINS -------- //

typedef struct _Chemin{ 
    int coordY;
    int coordX;
    int score;
    Direction startingDirection;
    struct _Chemin *next;
} Chemin; 



// =============== Fonctions =============== //  

// -> Creation <- //
Chemin* createChemin(int coordY,int coordX, int score,Direction startingDirection, Chemin *next );

// -> Getter <- // 
int getCheminCoordX(Chemin * chemin);
int getCheminCoordY(Chemin * chemin);
int getCheminScore(Chemin * chemin);
Direction getCheminStartingDirection(Chemin * chemin);
Chemin *getCheminNext(Chemin * chemin);

// -> Setter <- // 
void setBestDirection(Chemin * chemin,Direction newDirection, int startX, int centerX);

// -> Condition <- // 
bool isANewBestDirection(Direction currentDirection, Direction newDirection, int startX, int centerX);
bool isCheminAtCoord(int coordY, int coordX,Chemin *chemin);
bool isCheminEndExisting(int coordY, int coordX,int newScore, Direction newStartingDirection, Chemin * chemins, int startX, int centerX);

// -> Autres <- // 
Direction getBestStartingDirection(Chemin * chemins, int startX, int centerX);
Chemin* addChemin(Chemin *newChemin, Chemin * chemins);

// -> Destruction <- // 
void destroyChemin(Chemin *chemin);
void destroyAllChemin(Chemin *chemin);








#endif
