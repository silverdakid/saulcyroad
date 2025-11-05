#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <termios.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "joueur.h"
#include "ligne.h"
#include "partie.h"
#include "statique.h"
#include "ia.h"

#include "../helper.h"
#include "../modules/printer.h"
#include "../modules/renderer.h"


Partie* createPartie(int score, bool estEnCours, Ligne *lignes ,Joueur *joueur){
    Partie* newPartie = malloc(sizeof(Partie));

    newPartie->score = score;
    newPartie->estEnCours = estEnCours;
    newPartie->ligne = lignes;
    newPartie->joueur = joueur;

    return newPartie;
}

Partie* generatePartie(bool isAIAJoueur){
    Joueur* joueur =initialiseJoueur(POULET, isAIAJoueur);
    
    return createPartie(0,false,generateStartingLigne(joueur),joueur);
}

int getPartieScore(Partie * partie){
    if(partie == NULL )return -1;
    return  partie->score ;
}

bool isPartieEnCours(Partie * partie){
    if(partie == NULL )return false;
    return  partie->estEnCours ;
}

Joueur* getPartieJoueur(Partie * partie){
    if(partie == NULL )return NULL;
    return  partie->joueur ;
}

Ligne* getPartieLignes(Partie * partie){
    if(partie == NULL )return NULL;
    return  partie->ligne ;
}

void addScore (Partie * partie){
    if(partie == NULL )return ;
    partie->score = getPartieScore(partie) + 1 ;
} 


void updateEstEnCours(Partie * partie){
    if(partie == NULL) return;
    partie->estEnCours = !isPartieEnCours(partie);

} 

void moveAllDynamiquesInPartie(Partie* partie, bool moveSlow){  
    if(moveSlow){
        moveDynamiquesOnAllLignes(getPartieLignes(partie),getPartieJoueur(partie),true,1);
    }else{
        moveDynamiquesOnAllLignes(getPartieLignes(partie),getPartieJoueur(partie),false,1);
    }
        
    printAll(getPartieLignes(partie),getPartieJoueur(partie),getPartieScore(partie));
}


void playPartie(void){ 
    Partie * partie = generatePartie(makeChoice("Voulez-vous qu'une IA joue ? (y/n) : "));

    if(isAnIA(getPartieJoueur(partie))){
        playIA(partie);
        return;
    }
   
    playJoueur(partie);
}

void playJoueur( Partie * partie){
    printStartPartie(false);

    setNonBlockingInput();

    char ch;
    int bestScore = loadIntVariable("best_score");
    updateEstEnCours(partie);
    
    clock_t deltaFast = clock();
    clock_t deltaSlow = clock();

    printAll(getPartieLignes(partie),getPartieJoueur(partie),getPartieScore(partie));

    while (isAlive(getPartieJoueur(partie))) {
        ch = getchar(); 
        
        if(ch == ' ' ){  
            updateEstEnCours(partie);
            if(isPartieEnCours(partie)){
                setNonBlockingInput();
                printAll(getPartieLignes(partie),getPartieJoueur(partie),getPartieScore(partie));
                deltaSlow = clock();
                deltaFast = clock();
            }else{
                resetTerminal();
                printPausePartie(getPartieScore(partie),bestScore);
                setNonBlockingInput();
                continue;
            }
        }else if ( !isPartieEnCours(partie)) continue;

        if ( (clock() - deltaSlow) >= COEF_SLOW * CLOCKS_PER_SEC) { 
            moveAllDynamiquesInPartie(partie,true);
            deltaSlow = clock();
        }

        if ( (clock() - deltaFast) >= COEF_FAST * CLOCKS_PER_SEC) { 
            moveAllDynamiquesInPartie(partie,false);
            deltaFast = clock();
        }

        if (ch != EOF ) { 
            

            switch (ch){ 
            // ---- AVANCE --- // 
            case 'A' :
            case 'z' :
           
                if(isCaseAccessible(getPartieLignes(partie),getJoueurCoordY( getPartieJoueur(partie))+1,getJoueurCoordX( getPartieJoueur(partie))) ){
                    if(getJoueurBackwards(partie->joueur) == 0){
                        partie->ligne = generateNewLigne(getPartieLignes(partie),getPartieJoueur(partie),getPartieScore(partie));
                        addScore(partie);
                    }
                    moveJoueur(getPartieJoueur(partie),HAUT,false);
                }
                break;
            // ---- RECULE --- // 
            case 'B' :
            case 'w' :
                if(isCaseAccessible(getPartieLignes(partie),getJoueurCoordY( getPartieJoueur(partie))-1,getJoueurCoordX( getPartieJoueur(partie))) ){
                    moveJoueur(getPartieJoueur(partie),BAS,false);
                }
                break;
            // ---- GAUCHE --- // 
            case 'D' :
            case 'q' :
                if(isCaseAccessible(getPartieLignes(partie),getJoueurCoordY( getPartieJoueur(partie)),getJoueurCoordX( getPartieJoueur(partie))-1) ){
                    moveJoueur(getPartieJoueur(partie),GAUCHE,false);
                }
                break;
            // ---- DROITE --- // 
            case 'C' :
            case 'd' :
            if(isCaseAccessible(getPartieLignes(partie),getJoueurCoordY( getPartieJoueur(partie)),getJoueurCoordX( getPartieJoueur(partie))+1) ){
                    moveJoueur(getPartieJoueur(partie),DROITE,false);
                }
                break;
            
            default:
                break;
            }
            if(isColision(getPartieLignes(partie),getJoueurCoordY(getPartieJoueur(partie)),getJoueurCoordX(getPartieJoueur(partie)))) killJoueur(getPartieJoueur(partie));
            printAll(getPartieLignes(partie),getPartieJoueur(partie),getPartieScore(partie));
        }
        
    }
    resetTerminal();
    if( bestScore < getPartieScore(partie)) saveIntVariable(getPartieScore(partie),"best_score"); 
    
    printEndPartie(getPartieScore(partie), bestScore < getPartieScore(partie) ? getPartieScore(partie) : bestScore);

    free(partie);

    if(!makeChoice("")) return;
    
    playPartie();
}

void playIA(Partie * partie){
    printStartPartie(true); 
    updateEstEnCours(partie);

    int bestScore = loadIntVariable("best_score_ia");
    
    int bufferCoordY = getJoueurCoordY(getPartieJoueur(partie));
    
    clock_t deltaFast = clock();
    clock_t deltaSlow = clock();
    clock_t deltaIA = clock();
    clock_t deltaMove = clock();

    printAll(getPartieLignes(partie),getPartieJoueur(partie),getPartieScore(partie));

    while (isAlive(getPartieJoueur(partie))) {
        if ( (clock() - deltaSlow) >= COEF_SLOW * CLOCKS_PER_SEC) { 
            moveAllDynamiquesInPartie(partie,true);
            deltaSlow = clock();
        }

        if ( (clock() - deltaFast) >= COEF_FAST * CLOCKS_PER_SEC) { 
            moveAllDynamiquesInPartie(partie,false);
            deltaFast = clock();
        }

        if ( (clock() - deltaMove) >= COEF_MOVE * CLOCKS_PER_SEC) { 
            killJoueur(getPartieJoueur(partie));
        }

        if ( (clock() - deltaIA) >= COEF_IA * CLOCKS_PER_SEC) { 
            Direction direction = bestDirectionToGo(getPartieLignes(partie),getPartieJoueur(partie),deltaSlow);

            if(direction == HAUT && getJoueurBackwards(getPartieJoueur(partie)) == 0){
                partie->ligne = generateNewLigne(getPartieLignes(partie),getPartieJoueur(partie),getPartieScore(partie));
                addScore(partie);
            }

            moveJoueur(getPartieJoueur(partie),direction,false);
            
            if(direction == HAUT || direction == BAS || direction == GAUCHE || direction == DROITE){
                if(isColision(getPartieLignes(partie),getJoueurCoordY(getPartieJoueur(partie)),getJoueurCoordX(getPartieJoueur(partie)))) killJoueur(getPartieJoueur(partie));
                printAll(getPartieLignes(partie),getPartieJoueur(partie),getPartieScore(partie));
            } 

            deltaIA = clock();
            if( !(bufferCoordY == getJoueurCoordY(getPartieJoueur(partie)))){
                bufferCoordY = getJoueurCoordY(getPartieJoueur(partie));
                deltaMove = clock();
            }
        }
        
    }
    
    if( bestScore < getPartieScore(partie)) saveIntVariable(getPartieScore(partie),"best_score_ia"); 
    
    printEndPartie(getPartieScore(partie), bestScore < getPartieScore(partie) ? getPartieScore(partie) : bestScore);

    free(partie);

    if(!makeChoice("")) return;
    
    playPartie();
}


void destroyPartie(Partie *partie){
    if(partie == NULL)return;
    destroyJoueur(getPartieJoueur(partie));
    destroyAllLigne(getPartieLignes(partie));
    free(partie);
}




// Fonction principale de jeu SDL
void playPartieSdl(bool isAIControlled, SDLResources* resources) {

    // Utilisation des constantes définies dans partie.h
    const int WIDTH = SDL_WINDOW_WIDTH;
    const int HEIGHT = SDL_WINDOW_HEIGHT;
    const int NB_LIGNE = SDL_GRID_ROWS;
    const int NB_COLONNE = SDL_GRID_COLUMNS;

    resources->cellWidth = WIDTH / NB_COLONNE;
    resources->cellHeight = HEIGHT / NB_LIGNE;
    resources->isPlayerJumping = 0;
    resources->playerBounceOffset = 0;
    resources->playerBounceDirection = 0;
    

    // Initialiser la partie
    Partie *partie = generatePartie(isAIControlled);
    if (!partie || !partie->joueur || !partie->ligne) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur lors de la création de la partie");
        destroyPartie(partie);
        return;
    }

    updateEstEnCours(partie);

    int running = 1;
    int offset = 0;
    int ligne = 0; 
    clock_t deltaSlow = clock();
    clock_t deltaFast = clock();
    clock_t deltaIA = clock();

    int bestScore;
    char* score_key;

    if (isAIControlled) {
        score_key = "best_score_ia";
    } else {
        score_key = "best_score";
    }
    bestScore = loadIntVariable(score_key);

    // Démarrer la musique de fond
    resumeBackgroundMusic();
    
    while (running && isAlive(getPartieJoueur(partie))) {
        // Gestion des événements SDL, des entrées joueur et de la logique de l'IA
        handleEvents(partie, &running, &offset, &ligne, &deltaIA, &deltaSlow, &deltaFast, isAIControlled, resources);

        if (!running || !isAlive(getPartieJoueur(partie))) {
            break;
        }

        // Si la partie est en PAUSE
        if (!isPartieEnCours(partie)) {
            renderWorld(resources, partie, NB_LIGNE, NB_COLONNE, HEIGHT);
            renderPlayer(resources, partie, offset, ligne, NB_COLONNE, HEIGHT);
            renderPauseOverlay(resources->renderer, WIDTH, HEIGHT);
            SDL_RenderPresent(resources->renderer);
            SDL_Delay(16);
            continue;
        }
        
        updateGameState(partie, &deltaSlow, &deltaFast); 
        
        if (isAlive(getPartieJoueur(partie)) && isColision(getPartieLignes(partie),getJoueurCoordY(getPartieJoueur(partie)),getJoueurCoordX(getPartieJoueur(partie)))){
            renderDeathAnimation(resources, partie);
            killJoueur(getPartieJoueur(partie));
        }

        if (isPartieEnCours(partie) && isAlive(getPartieJoueur(partie))) {
            renderWorld(resources, partie, NB_LIGNE, NB_COLONNE, HEIGHT);
            renderPlayer(resources, partie, offset, ligne, NB_COLONNE, HEIGHT);
            SDL_RenderPresent(resources->renderer);
        }
        
        SDL_Delay(16);
    }
    
    // Sauvegarder le score
    int finalScore = getPartieScore(partie);
    if (bestScore < finalScore) {
        saveIntVariable(finalScore, score_key); // Utilise la bonne clé de score
        bestScore = finalScore;
    }
    
    SDL_bool playAgain = showEndScreen(finalScore, bestScore, resources);
    destroyPartie(partie);
    
    if (playAgain) {
        playPartieSdl(isAIControlled, resources); // Relance la partie avec le même mode de contrôle
    }
}

// Fonction pour mettre à jour l'état du jeu
void updateGameState(Partie* partie, clock_t* deltaSlow, clock_t* deltaFast) {
    if (!isPartieEnCours(partie)) {
        return;
    }

    // Gestion du mouvement lent des objets dynamiques
    if ((clock() - *deltaSlow) >= (COEF_SLOW * CLOCKS_PER_SEC) / 8) { 
        moveAllDynamiquesInPartie(partie, true);
        *deltaSlow = clock();
    }

    // Gestion du mouvement rapide des objets dynamiques
    if ((clock() - *deltaFast) >= (COEF_FAST * CLOCKS_PER_SEC) / 8) { 
        moveAllDynamiquesInPartie(partie, false);
        *deltaFast = clock();
    }
}

void showGraphicalMenu(void) {
    const int WIDTH = SDL_WINDOW_WIDTH;
    const int HEIGHT = SDL_WINDOW_HEIGHT;

    SDLResources resources;
    resources.cellWidth = WIDTH / SDL_GRID_COLUMNS;
    resources.cellHeight = HEIGHT / SDL_GRID_ROWS;
    

    if (!initSDL(&resources, WIDTH, HEIGHT)) {
        return;
    }
    
    if (!loadAllTextures(&resources)) {
        cleanupSDL(&resources);
        return;
    }
    
    playBackgroundMusic(&resources.audio);
    
    int menuChoice;
    bool menuRunning = true;
    
    while (menuRunning) {
        menuChoice = showMainMenu(&resources);
        
        switch (menuChoice) {
            case 1: // Jouer
                pauseBackgroundMusic();
                playPartieSdl(false, &resources);
                resumeBackgroundMusic();
                break;

            case 2: // Mode IA
                pauseBackgroundMusic();
                playPartieSdl(true, &resources);
                resumeBackgroundMusic();
                break;

            case 3: // Vestiaire
                showWardrobe(&resources);
                freeAllTextures(&resources.textures);
                if (!loadAllTextures(&resources)) {
                    menuRunning = false;
                }
                break;

            case 0: // Quitter
            default:
                menuRunning = false;
                break;
        }
    }
    Mix_HaltMusic();
    cleanupSDL(&resources);
}
