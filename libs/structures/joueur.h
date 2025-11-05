#ifndef JOUEUR
#define JOUEUR

#define LIMIT_BACKWARD 5
#define LIMIT_JOUEUR 15


#include <stdbool.h>

#include "../types/types.h"


// -------- CLASS POUR LE JOUEUR -------- //

typedef struct _Joueur{ 
    int coordY;
    int coordX;
    int limites;
    int backwards;
    bool isAnIA;
    bool estEnVie;
    TypeJoueur type;
} Joueur; 

// =============== Fonctions =============== //  

// -> Creation/Initialisation <- //
Joueur* createJoueur(int coordY,int coordX ,int limites,int backwards , TypeJoueur type , bool estEnVie, bool isAnIA );
Joueur* initialiseJoueur( TypeJoueur type , bool isAnIA ); 

// -> Getter <- //
int getJoueurCoordX(Joueur * joueur);
int getJoueurCoordY(Joueur * joueur);
int getJoueurLimites(Joueur * joueur);
int getJoueurBackwards(Joueur * joueur); 
TypeJoueur getJoueurType(Joueur * joueur);
bool isAlive(Joueur * joueur);
bool isAnIA(Joueur * joueur);

// -> Setter <- //
void reduceBackward(Joueur * joueur);
bool addBackward(Joueur * joueur);

void setJoueurCoordX(Joueur * joueur,int coordX );
void setJoueurCoordY(Joueur * joueur,int coordY );

void killJoueur(Joueur * joueur);

// -> Condition <- //
bool willBeInsideLimites(int newCoordX, int joueurLimites);  

// -> Déplacement du joueur <- //
void moveJoueur(Joueur *joueur, Direction direction,bool couldGoOutsideLimite);

// -> toString <- //
char joueurToString(Joueur *joueur); 

// -> Destruction <- //
void destroyJoueur(Joueur *joueur);

// -> Copy du Joueur <- // 
Joueur * copyJoueur(Joueur* joueur);


#endif
