#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ligne.h"
#include "chemin.h"
#include "joueur.h"
#include "../types/types.h"

Ligne* createLigne(int coordY, TypeLigne type , Direction direction , Statique *statiques , Dynamique *dynamiques,Ligne * next){
    Ligne* newLigne = malloc(sizeof(Ligne));

    newLigne->coordY= coordY;
    newLigne->type= type;
    newLigne->direction= direction;
    newLigne->statiques= statiques;
    newLigne->dynamiques=dynamiques;
    newLigne->next=next;

    return newLigne;
}

int getLigneCoordY(Ligne * ligne){
    if(ligne == NULL )return -1;
    return  ligne->coordY ;
}

TypeLigne getLigneType(Ligne * ligne){
    if(ligne == NULL )return HERBE;
    return  ligne->type ;
}

Direction getLigneDirection(Ligne * ligne){
    if(ligne == NULL )return GAUCHE;
    return  ligne->direction ;
}

Statique *getLigneStatique(Ligne * ligne){
    if(ligne == NULL )return NULL;
    return  ligne->statiques ;
}

Dynamique *getLigneDynamique(Ligne * ligne){
    if(ligne == NULL )return NULL;
    return  ligne->dynamiques ;
}

Ligne* getLigneNext(Ligne * ligne){
    if(ligne == NULL )return NULL;
    return ligne->next;
}

bool isLigneDeadly(Ligne * ligne){
    if(ligne == NULL )return false;
    return getLigneType(ligne) == FLEUVE || getLigneType(ligne) == RAPIDE;
}

bool isLigneStatique(Ligne *ligne){
    TypeLigne ligneType=  getLigneType(ligne);
    return ligneType == FLEUVE || ligneType == HERBE;
}

bool isLigneSlow(Ligne *ligne){ 
    TypeLigne ligneType=  getLigneType(ligne);
    return ligneType != VOIE_FERREE ;
}

bool isLigneAtCoordY(Ligne *ligne, int coordY){
    if(ligne == NULL )return false;
    return getLigneCoordY(ligne) == coordY;
}


bool isColision(Ligne * lignes,int coordY, int coordX){ 
    Ligne* current = lignes;

    while (current != NULL) {
        Ligne* buffer =current;
        current = current->next;  
        
        if (isLigneAtCoordY(buffer, coordY)) {
            if (isLigneDeadly(buffer)) {
                bool statique = isStatiqueExists(getLigneStatique(buffer), coordX, isStatiqueAtCoord);
                bool dynamique = isOneDynamiqueAtCoord(getLigneDynamique(buffer), getLigneDirection(buffer), coordX);
                return !(statique || dynamique);
            } else if (!isLigneStatique(buffer)) {
                return isOneDynamiqueAtCoord(getLigneDynamique(buffer), getLigneDirection(buffer), coordX);
            }
            return false;
        }
        
    }
    return false;
}

Ligne* removeFirst(Ligne * lines){
    if(lines == NULL) return NULL;
    Ligne * nextLigne = getLigneNext(lines);
    destroyLigne(lines);
    return nextLigne;
}

Ligne* getLastLigne(Ligne * lignes){
    if(lignes == NULL) return lignes;
    Ligne* current = lignes;

    while (getLigneNext(current) != NULL) {
        current = getLigneNext(current);  
    }

    return current;
}

Ligne* getBeforeLastLigne(Ligne * lignes){  
    if(lignes == NULL  ) return lignes;
    Ligne* current = lignes;

    while ( getLigneNext( getLigneNext(current)) != NULL) {
        current = getLigneNext(current);  
    }

    return current;
}

Ligne* getLastStatiqueLigne(Ligne * lignes){ 
    if(lignes == NULL) return NULL;
    Ligne* lastStatique = NULL;
    Ligne* current = lignes;

    while (current->next != NULL) {
        if(isLigneStatique(current) && lastStatique== NULL) lastStatique = current;
        else lastStatique = NULL;
        current = current->next;  
        
    }

    return lastStatique;
}

Ligne* forEachLigne(Ligne * lignes, void (*f)(Ligne*)){
    Ligne* current = lignes;

    while (current != NULL) {
        Ligne* next = current->next;
        f(current);
        current = next;
    }

    return lignes;
}

Ligne* getLigneAtCoordY(Ligne * lignes, int coordY){
    Ligne* current = lignes;

    while (current != NULL) {
        if(getLigneCoordY(current) == coordY) return current;
        current = current->next;  
    }

    return NULL;
}


void moveDynamiquesOnAllLignes(Ligne * lignes,Joueur * joueur, bool moveSlow, int nbTimes){
    if(joueur == NULL) return;

    Ligne* current = lignes;
    int endLignes = LINE_SIZE(getJoueurLimites(joueur))  ;

    while (current != NULL) {
        Ligne* buffer =current;
        current = current->next;  

        if(isLigneStatique(buffer) || isLigneSlow(buffer)!=moveSlow) continue;

        Direction ligneDirection = getLigneDirection(buffer);
        
        // -> Déplacement des dynamiques <-//
        buffer->dynamiques = moveAllDynamique(getLigneDynamique(buffer),ligneDirection, endLignes,nbTimes);

        // -> Déplacement du joueur si sur une buche <- //
        if( getLigneType(buffer) == RAPIDE && getLigneCoordY(buffer)== getJoueurCoordY(joueur)) moveJoueur(joueur,ligneDirection,true) ;

        int size = LINE_SIZE(getJoueurLimites(joueur));
        int startX = ligneDirection == DROITE ? -1 - size /2 : endLignes + size/2 ;
        if(isANewDynamiqueOnLigne(getLigneType(buffer),getLigneDynamique(buffer)) ) {
            if( 
                (getLigneDirection(buffer) == GAUCHE && isOneDynamiqueAtCoord(getLigneDynamique(buffer),ligneDirection,startX) )
                || 
                (getLigneDirection(buffer) == DROITE && isDynamiqueAtCoord(getLigneDynamique(buffer),ligneDirection,startX)))
                 continue;
            
            TypeDynamique typeDyn = generateDynamiqueFromType(getLigneType(buffer));
            int dynamiqueSize = generateSizeOfDynamique(typeDyn);
 
            if(isOneDynamiqueAtTheseCoord(getLigneDynamique(buffer),ligneDirection,startX,dynamiqueSize)) continue;

            buffer->dynamiques = addDynamique(getLigneDynamique(buffer),createDynamique(startX,dynamiqueSize, typeDyn == TRAIN ? 40 : 0,typeDyn,NULL));
        }
    }
}

TypeLigne generateLigneType(Ligne *lignes){
    Ligne *current = getLastLigne(lignes);
    float * probability = typeLigneProbability(getLigneType(current));

    float alea = (float)rand() / RAND_MAX;  
    float acc = 0.0;
    
    for (int idx = 0; idx < NB_TYPELIGNE; idx++) {
        acc += probability[idx];
        if (alea <= acc) {
            return (TypeLigne)idx;
        }
    }
    return HERBE;
}

Ligne* generateStartingLigne( Joueur * joueur){
    if(joueur == NULL) return NULL;
    int size = LINE_SIZE(getJoueurLimites(joueur));


    Statique * newStat = createStatique(0,generateStatiqueFromType(HERBE),NULL);
    for(int jdx=1; jdx < size  ; jdx++){
        newStat = addStatique(newStat,createStatique(jdx,generateStatiqueFromType(HERBE),NULL));
    }
    Ligne * lignes = createLigne(0,HERBE,GAUCHE,newStat,NULL,NULL);
    Ligne * current = lignes;

    for(int idx=0; idx < 5; idx++){ 
        Ligne * buffer = current;
        Statique * newStat = NULL;
        for(int jdx=0; jdx < size; jdx++){
            newStat = addStatique(newStat,createStatique(jdx,generateStatiqueFromType(HERBE),NULL));
        }
        current = createLigne(getLigneCoordY(buffer)+1,HERBE,GAUCHE,newStat,NULL,NULL);
        buffer->next = current; 
    }

    for(int idx=0; idx < 5; idx++){
        Ligne * buffer = current;
        current = createLigne(getLigneCoordY(buffer)+1,HERBE,GAUCHE,idx == 2 ? NULL: generateStatique(HERBE,size,-1),NULL,NULL);
        buffer->next = current;
        
    }
    
    for(int idx=0; idx < LINE_HEIGHT-9; idx++){
        current = generateNewLigne(current,joueur,0);
    }

    return lignes;
}

Ligne* generateNewLigne(Ligne *lignes, Joueur * joueur, int score){ 
    if(lignes == NULL ) return NULL; 
    
    Ligne *lastLigne = getLastLigne(lignes);
    TypeLigne newType = generateLigneType(lastLigne); 

    if (
        getLigneType(lastLigne) == RAPIDE &&
        getLigneType(getBeforeLastLigne(lignes)) == RAPIDE &&
        newType == RAPIDE) {
        newType = FLEUVE;
    }

    Ligne * newLigne= createLigne(getLigneCoordY(lastLigne)+1,newType,GAUCHE,NULL,NULL,NULL);
    getLastLigne(lignes)->next=newLigne; 

    int size = LINE_SIZE(getJoueurLimites(joueur));

    Ligne* toReturn = lignes->next;

    if( isLigneStatique(newLigne)){  
        newLigne->statiques = generateStatique(newType,size,score);
        
        Ligne* lastStatiqueLigne = getLastStatiqueLigne(lignes);
        Chemin* chemins = NULL;
        int joueurLimites = getJoueurLimites(joueur);
        int joueurLimiteY = joueurLimites+LIMITE_AFFICHAGE-1;

        int searchY = getLigneCoordY(lastLigne);

        for(int idx = LIMITE_AFFICHAGE-1; idx < joueurLimiteY; idx++){ 
            Chemin* currentChemin = createChemin(getLigneCoordY(lastStatiqueLigne),idx,0,HAUT,NULL);
            chemins = addChemin(createChemin(getLigneCoordY(lastStatiqueLigne),idx,0,HAUT,NULL),chemins);
            
            while(currentChemin != NULL){
                // -> typedef enum { GAUCHE, DROITE, HAUT, BAS , NB_DIRECTIONS } Direction; <- // 
                // => Jusque bas comme ça on a que GAUCHE,DROITE, HAUT <= //
                for (int direction = -1; direction < BAS; direction++) { 
                    int coordX = getCheminCoordX(currentChemin) + getDirectionNextCoordX(direction);
                    int coordY = getCheminCoordY(currentChemin) + getDirectionNextCoordY(direction); 
                    
                    // -> Vérifie si le chemin est déjà enregistré <- //
                    if( isCheminEndExisting(coordY,coordX, 0, HAUT , chemins,  0,  0)) continue;
                    
                    // -> Vérification de l'accessibilité <- //
                    if ( !isColision( newLigne, coordY, coordX) || !isCaseAccessible( newLigne, coordY, coordX) || !willBeInsideLimites(coordX,joueurLimites)) continue; 

                    // -> Si sur les coordonnées de la dernières lignes <-//
                    if(coordY == searchY+1){
                        destroyAllChemin(chemins);
                        destroyAllChemin(currentChemin);
                        return toReturn;
                    }

                    // -> Ajout de chemin dans les existants <- //
                    chemins = addChemin(createChemin(coordY,coordX, 0 , HAUT,NULL),chemins);

                    // -> Ajout de chemin sur le next du current <-//
                    Chemin * newChemin = createChemin(coordY,coordX, 0 , HAUT,currentChemin->next );
                    currentChemin->next = newChemin;

                } 
                Chemin * buffer = currentChemin;
                currentChemin= currentChemin->next; 
                destroyChemin(buffer);
            }
        }

        // -> On part du principe qu'on peut accéder à la dernière ligne de statique ( par récursion sur cette fonction ) <- //

        
        

        // -> Suppresion de l'obstacle au dessus ou rajouter si c'est un rapide<- //
        if(newType == FLEUVE){
            Chemin *current=chemins;
            Chemin *previous = NULL;

            while (current != NULL) {
                if (getCheminCoordY(current) == searchY) {
                    addStatique(newLigne->statiques,createStatique(getCheminCoordX(current),NENUPHARE,NULL));
                    previous = current;
                    current = current->next;
                } else {
                    Chemin *toDelete = current;
                    current = current->next;
                    if (previous == NULL) chemins = current;
                    else previous->next = current;
                    destroyChemin(toDelete);
                }
            }
        }else {
            // -> Suppresion lds chemins pour garder que ceux qui ont le même Y lastLigne et comptage <- //
            Chemin *current=chemins;
            Chemin *previous = NULL;

            while (current != NULL) {
                if (getCheminCoordY(current) == searchY) {
                    newLigne->statiques = removeStatiqueAtCoord(newLigne->statiques,getCheminCoordX(current));
                    previous = current;
                    current = current->next;
                } else {
                    Chemin *toDelete = current;
                    current = current->next;
                    if (previous == NULL) chemins = current;
                    else previous->next = current;
                    destroyChemin(toDelete);
                }
            }

         
        }

        destroyAllChemin(chemins);

    }else{
        newLigne->direction = generateDirectionForDynamique();
        newLigne->dynamiques = generateDynamique(newType,size,score,getLigneDirection(newLigne)); 
        
    }
    
    return toReturn; 
}

Ligne * copyLigne(Ligne *ligne){ 
    if(ligne == NULL) return NULL;
    return createLigne(
        getLigneCoordY(ligne),
        getLigneType(ligne),
        getLigneDirection(ligne),
        copyStatiques(getLigneStatique(ligne)), 
        copyDynamiques(getLigneDynamique(ligne)),
        NULL
    );
}

Ligne * copyLignes(Ligne* lignes){ 
    Ligne * copy = copyLigne(lignes);

    Ligne * current = getLigneNext(lignes);
    Ligne * previous = copy;
    while(current != NULL){
        Ligne * buffer= copyLigne(current);
        previous->next = buffer;
        previous = buffer;
        current = getLigneNext(current);
    }
    return copy;
}

void destroyLigne(Ligne *ligne){
    if(ligne == NULL) return;
    destroyAllDynamique(getLigneDynamique(ligne));
    destroyAllStatique(getLigneStatique(ligne));
    free(ligne);
}

void destroyAllLigne(Ligne *ligne){
    forEachLigne(ligne,destroyLigne);
}

char* ligneToString(Ligne *ligne, Joueur *joueur){
    if (ligne == NULL || joueur == NULL) return "\n";
    
    int joueurX = getLigneCoordY(ligne) == getJoueurCoordY(joueur) ? getJoueurCoordX(joueur) : -1;
    char joueurCara = typeJoueurToString(getJoueurType(joueur)); 

    int limite = getJoueurLimites(joueur);
    Direction ligneDirection=  getLigneDirection(ligne);
    char ligneCara = typeLigneToString(getLigneType(ligne)); 
    
    int taille = LINE_SIZE(limite);
    char *string = malloc(sizeof(char)*(taille+3));

    int acc=0;

    Statique* currentStatique= getLigneStatique(ligne);
    Dynamique* currentDynamique= getLigneDynamique(ligne);

    // ----- OPTENTION DU PREMIER DYNAMIQUE DANS LES COORDONNÉES ----- //  
    while( currentDynamique!=NULL && getDynamiqueCoordX(currentDynamique) < 0 && ligneDirection == DROITE ){
        currentDynamique = getDynamiqueNext(currentDynamique);
    }

    for(int idx =0; idx < taille; idx ++) {
        if(idx == LIMITE_AFFICHAGE || idx == LIMITE_AFFICHAGE+limite ){
            string[idx+acc]='|';
            acc++;
        }  
        
        // ----- STATIQUE ----- //
        if( isLigneStatique(ligne) && isStatiqueAtCoord(currentStatique,idx) ){
            string[idx+acc] =  statiqueToString(currentStatique);
            currentStatique = getStatiqueNext(currentStatique);
        }
        // ----- DYNAMIQUE ----- //
        else if( !isLigneStatique(ligne) && isDynamiqueAtCoord(currentDynamique,ligneDirection,idx)) {
                string[idx+acc]=  typeDynamiqueToString(getDynamiqueType(currentDynamique),ligneDirection);
                if(
                    (getDynamiqueCoordX(currentDynamique)+getDynamiqueLongueur(currentDynamique)-1 == idx && ligneDirection == GAUCHE) || 
                    (getDynamiqueCoordX(currentDynamique)==idx && ligneDirection==DROITE)
                ){ 
                    currentDynamique = getDynamiqueNext(currentDynamique);
                }
        }else if( getLigneType(ligne) == VOIE_FERREE && getDynamiqueTicBeforeMove(getLigneDynamique(ligne)) != 0 && (idx+acc)%3 == 0){
            char warningCara = '!';
            string[idx+acc]=  warningCara;
        }else{
            string[idx+acc]=  ligneCara;
        }
        // ----- JOUEUR ----- //
        if( joueurX == idx ){
            string[idx+acc]= joueurCara;
            
        }
    } 
    string[taille+2] = '\0';

    return string;
}

bool isCaseAccessible(Ligne * lignes, int newY, int newX){ 
    if(lignes == NULL ) return false;
    Ligne * current = getLigneAtCoordY(lignes,newY) ;
    bool isStatique = isStatiqueExists(getLigneStatique(current),newX,isStatiqueAtCoord);
    return !isStatique || isLigneDeadly(current);
}

void printAll(Ligne *lignes, Joueur *joueur, int score){
    Ligne* current = lignes;
   
    int hideSize = LIMIT_BACKWARD - getJoueurBackwards(joueur); 
    for(int idx=0; idx < hideSize; idx ++){
        if(current != NULL) current = current->next;  
    }
    char** tableau = malloc(HEIGHT_AFFICHAGE * sizeof(char*));
    
    for(int idx=0; idx < HEIGHT_AFFICHAGE; idx ++){
        if(current ==NULL) break;  
        tableau[idx] =ligneToString(current,joueur);
        current = current->next;  
    }
    printf("                                            %s                  Votre score : %d\n",tableau[HEIGHT_AFFICHAGE-1],score);
    
    for(int idx=HEIGHT_AFFICHAGE-2; idx >= 0; idx --){
        printf("                                            %s\n",tableau[idx]);
        free(tableau[idx]);
    }
    free(tableau);
    printf("════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n \n"    );


}

