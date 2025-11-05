#ifndef MAPCHEMIN
#define MAPCHEMIN

#include <stdbool.h>

#include "chemin.h"


// -------- CLASS D'UNE MAP DE CHEMINS -------- //

typedef struct _MapChemins{ 
    int size;
    Chemin **map;
} MapChemins;

// =============== Fonctions =============== //  

// -> Creation <- //
MapChemins* createMapChemins(int size );

// -> Getter <- //
int getMapCheminsSize(MapChemins * mapChemins);
Chemin **getMapCheminsMap(MapChemins * mapChemins);
Chemin *getMapCheminsAtIndex(MapChemins * mapChemins,int index); 

// -> Ajout <- //
void addCheminInMapChemins(Chemin *newChemin, MapChemins * mapChemins,int index); 

// -> Destruction <- //
void destroyMapChemins(MapChemins * mapChemins);

#endif

