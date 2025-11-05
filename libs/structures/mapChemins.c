#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "chemin.h"
#include "mapChemins.h"
#include "../types/types.h"

MapChemins* createMapChemins(int size ){
    if(size <= 0) return NULL;
    
    MapChemins* newMapChemin = malloc(sizeof(MapChemins));

    newMapChemin->size= size;
    newMapChemin->map = malloc( sizeof(Chemin)*size+1);
    
    for(int idx = 0; idx < size; idx ++){
        newMapChemin->map[idx]=NULL;
    }
    
    return newMapChemin;
}

int getMapCheminsSize(MapChemins * mapChemins){
    if(mapChemins == NULL) return -1;
    return mapChemins->size;
}

Chemin **getMapCheminsMap(MapChemins * mapChemins){
    if(mapChemins == NULL) return NULL;
    return mapChemins->map;
}

Chemin *getMapCheminsAtIndex(MapChemins * mapChemins,int index){
    if(mapChemins == NULL || index < 0 || index >= getMapCheminsSize(mapChemins)) return NULL;
    return mapChemins->map[index];
}

void addCheminInMapChemins(Chemin *newChemin, MapChemins * mapChemins,int index){
    if (mapChemins == NULL || newChemin == NULL || index < 0 || index >= getMapCheminsSize(mapChemins)) return ;

    mapChemins->map[index]= addChemin(newChemin,mapChemins->map[index]);
}

void destroyMapChemins(MapChemins * mapChemins){
    if(mapChemins == NULL) return;
    int size = getMapCheminsSize(mapChemins);

    for(int idx = 0; idx < size; idx ++){
        destroyAllChemin(mapChemins->map[idx]);
    }
    free(mapChemins->map);
    free(mapChemins);
}




