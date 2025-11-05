#ifndef LIGNE
#define LIGNE

#define COEF_MOVE 20
#define COEF_SLOW 1
#define COEF_FAST 0.1
#define COEF_IA 0.3

#define LIMITE_AFFICHAGE 4
#define HEIGHT_AFFICHAGE 15
#define LINE_SIZE(joueurLimite) ((joueurLimite) + (LIMITE_AFFICHAGE * 2))
#define LINE_HEIGHT (HEIGHT_AFFICHAGE+LIMIT_BACKWARD)

#define STARTING_Y 8 
#define STARTING_X(limitX_joueur) ((LIMITE_AFFICHAGE - 1) + ((limitX_joueur) / 2) + 1) 


#include <stdbool.h>

#include "dynamique.h"
#include "statique.h"
#include "joueur.h"
#include "../types/types.h"

// -------- CLASS POUR LES LIGNES DU PLATEAU ( en liste chaîné ) -------- //


typedef struct _Ligne{ 
    int coordY;
    TypeLigne type;

    Direction direction;
    Statique *statiques;
    Dynamique *dynamiques;

    struct _Ligne *next;
} Ligne;

// =============== Fonctions =============== // 

// -> Creation  <- // 
Ligne* createLigne(int coordY, TypeLigne type , Direction direction , Statique *statiques , Dynamique *dynamiques,Ligne * next);

// -> Getter  <- // 
int getLigneCoordY(Ligne * ligne);
TypeLigne getLigneType(Ligne * ligne);
Direction getLigneDirection(Ligne * ligne);
Statique *getLigneStatique(Ligne * ligne);
Dynamique *getLigneDynamique(Ligne * ligne);
Ligne* getLigneNext(Ligne * ligne);
Ligne* getLigneAtCoordY(Ligne * lignes, int coordY);

// -> Conditions  <- //
bool isLigneDeadly(Ligne * ligne);
bool isLigneStatique(Ligne *ligne);
bool isLigneAtCoordY(Ligne *ligne, int coordY);
bool isColision(Ligne * lignes, int coordY, int coordX); 
bool isLigneSlow(Ligne *ligne);
bool isCaseAccessible(Ligne * lignes, int newY, int newX); 

// -> Suppression et obtenir de valeurs  <- 
Ligne* removeFirst(Ligne * lines); 
Ligne* getLastLigne(Ligne * lignes);
Ligne* getBeforeLastLigne(Ligne * lignes);
Ligne* getLastStatiqueLigne(Ligne * lignes);

// -> Prog fonctionnelle   <- // 
Ligne* forEachLigne(Ligne * lignes, void (*f)(Ligne*));

// -> Déplacement des objects  <- // 
void moveDynamiquesOnAllLignes(Ligne * lignes,Joueur * joueur, bool moveSlow, int nbTimes);

// -> Génération  <- // 
TypeLigne generateLigneType(Ligne *lignes);
Ligne* generateStartingLigne( Joueur * joueur);
Ligne* generateNewLigne(Ligne *lignes, Joueur * joueur, int score);

// -> toString  <- // 
char* ligneToString(Ligne *ligne, Joueur *joueur);
void printAll(Ligne *lignes, Joueur *joueur, int score);

// -> copy  <- // 
Ligne * copyLigne(Ligne *ligne);
Ligne * copyLignes(Ligne* lignes);

// -> Destruction  <- // 
void destroyLigne(Ligne *ligne);
void destroyAllLigne(Ligne *ligne);

#endif
