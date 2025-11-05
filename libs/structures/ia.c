
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <time.h>


#include "ia.h"
#include "plateau.h"
#include "ligne.h"
#include "joueur.h"

#include "chemin.h"
#include "mapChemins.h"

#include "../types/types.h"


// -> Équilibre entre le plus proche du centre et le plus en avant <- //
int notePosition(int startX, int startY, int endX, int endY, int centerX){
    int forwardNote = endY - startY;
    int centerGain = abs(centerX - startX) - abs(centerX - endX); 
    int backward = (forwardNote < 0) ? -5 : 0;

    return 1000 * forwardNote + 10 * centerGain + backward;
}


// -> On joue toutes les COEF_SLOW/2 s le joueur <- //
Direction bestDirectionToGo(Ligne* lignes, Joueur* joueur,clock_t currentClock){ 
    Joueur *currentJoueur = copyJoueur(joueur); 

    double bufferClock = (double)(clock() - currentClock) / CLOCKS_PER_SEC ;

    // -> Tableau pour stocker les différents plateaux qui arrivent <- //
    Plateau *plateau = generatePlateaux(lignes,PROFONDEUR,bufferClock,currentJoueur);

    // -> Tableau pour stocker les différents chemins ( éviter des calculs inutiles) <- //
    MapChemins* mapChemins = createMapChemins(PROFONDEUR); 

    int startX = getJoueurCoordX(joueur);
    int startY = getJoueurCoordY(joueur);
    int joueurLimites = getJoueurLimites(joueur);
    int joueurBackwards = getJoueurBackwards(joueur);
    int centerX = STARTING_X(joueurLimites);

    // -> Exploration des directions possibles <- //
    for(int idx = 0; idx < PROFONDEUR; idx++){
        Chemin * current = idx == 0 ? createChemin(startY,startX,0,HAUT,NULL) : getMapCheminsAtIndex(mapChemins, idx-1) ;
        while( current != NULL){
            for (int direction = -1; direction < NB_DIRECTIONS; direction++) {

                int coordX = getCheminCoordX(current) + getDirectionNextCoordX(direction);
                int coordY = getCheminCoordY(current) + getDirectionNextCoordY(direction);                     

                if( startY - 3 + joueurBackwards  > coordY ) continue; 

                if(isColision(getPlateauLigne(plateau), coordY, coordX) || !isCaseAccessible( getPlateauLigne(plateau), coordY, coordX) || !willBeInsideLimites(coordX,joueurLimites)) continue; 
                if(isColision(getPlateauLigne(getPlateauNext(plateau)), coordY, coordX) || !isCaseAccessible( getPlateauLigne(getPlateauNext(plateau)), coordY, coordX) ) continue; 
              
                if(haveDynamiquesMoves(getPlateauNext(plateau))){
                    Ligne * ligne = getLigneAtCoordY(getPlateauLigne(plateau),coordY);
                    if(RAPIDE == getLigneType(ligne)){
                        int moveX = coordX + getDirectionNextCoordX(getLigneDirection(lignes)); 
                        if(isColision(getPlateauLigne(plateau), coordY, moveX) || !isCaseAccessible( getPlateauLigne(plateau), coordY, moveX) || !willBeInsideLimites(moveX,joueurLimites)) continue; 
                        if(isColision(getPlateauLigne(getPlateauNext(plateau)), coordY, moveX) || !isCaseAccessible( getPlateauLigne(getPlateauNext(plateau)), coordY, moveX)) continue; 
                    }  
                }

                int score = notePosition(startX, startY, coordX, coordY, centerX) + getCheminScore(current);

                if( !isCheminEndExisting(coordY,coordX, score,idx == 0 ? direction : getCheminStartingDirection(current) , getMapCheminsAtIndex(mapChemins,idx),  startX,  centerX)){
                    addCheminInMapChemins(createChemin(coordY,coordX, score , idx == 0 ? direction : getCheminStartingDirection(current),NULL),mapChemins,idx);
                }
            } 
            current = getCheminNext(current);
        }
        plateau = removeFirstPlateau(plateau);
    }

    Direction bestDirection = getCheminStartingDirection(getMapCheminsAtIndex(mapChemins,PROFONDEUR- 1));
    
    //-> Libération de tout <- //
    destroyJoueur(currentJoueur);
    destroyAllPlateau(plateau);
    destroyMapChemins(mapChemins); 

    //-> Retourner la meilleur direction <- //
    return bestDirection; 
}
