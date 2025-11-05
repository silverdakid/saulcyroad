
#ifndef TYPES
#define TYPES

#define COEFDIFFICULTY 50


typedef enum { NENUPHARE, BUISSON, ARBRE, } TypeStatique; 

typedef enum { VOITURE, CAMION, TRAIN, BUCHE, } TypeDynamique; 

typedef enum { ROUTE, HERBE, VOIE_FERREE, FLEUVE, RAPIDE, NB_TYPELIGNE } TypeLigne; 

typedef enum { POULET, CANARD,  } TypeJoueur; 

typedef enum { GAUCHE, DROITE, HAUT, BAS , NB_DIRECTIONS } Direction;


// -------- CONVERTIR LES TYPES EN STRING -------- //

char typeStatiqueToString(TypeStatique t);
char typeDynamiqueToString(TypeDynamique t,Direction direction);
char typeLigneToString(TypeLigne t);
char typeJoueurToString(TypeJoueur t);


// -------- GÉNÉRATION DES DIFFÉRENTS TYPES -------- // 

float * typeLigneProbability(TypeLigne type);
int numberOfElement(TypeLigne type, int size, int score);

TypeStatique generateStatiqueFromType(TypeLigne type);
TypeDynamique generateDynamiqueFromType(TypeLigne type);

Direction generateDirectionForDynamique(void);

// -------- Convertion du Type direction en Coordonnées -------- // 

int getDirectionNextCoordX(Direction direction);
int getDirectionNextCoordY(Direction direction);
#endif



