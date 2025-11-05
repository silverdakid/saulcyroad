#ifndef PARTIE
#define PARTIE

#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>

#include "ligne.h"
#include "joueur.h"

// -------- CONSTANTES POUR LA PARTIE SDL -------- //
#define SDL_WINDOW_WIDTH 800
#define SDL_WINDOW_HEIGHT 1000
#define SDL_GRID_ROWS 15
#define SDL_GRID_COLUMNS 15

// -------- CLASS POUR UNE PARTIE -------- //

typedef struct _Partie{ 
    int score;
    bool estEnCours;
    Ligne *ligne;
    Joueur *joueur;

} Partie; 

// =============== Fonctions =============== // 

// -> Creation / Generation <- //
Partie* createPartie(int score, bool estEnCours, Ligne *lignes ,Joueur *joueur); 
Partie* generatePartie(bool isAIAJoueur);

// -> Getter <- //
int getPartieScore(Partie * partie); 
bool isPartieEnCours(Partie * partie); 
Joueur* getPartieJoueur(Partie * partie); 
Ligne* getPartieLignes(Partie * partie); 

// -> Setter <- //
void addScore (Partie * partie); 
void updateEstEnCours (Partie * partie); 

// -> Jouer une partie <- //
void moveAllDynamiquesInPartie(Partie* partie, bool moveSlow);
void playPartie(void); 
void playJoueur( Partie * partie);
void playIA(Partie * partie);

// -> Destruction <- //
void destroyPartie(Partie *partie); 

// -------- Structure pour regrouper les textures du jeu -------- // 
typedef struct {
    SDL_Texture* joueur;
    SDL_Texture* buche;
    SDL_Texture* voiture_1;
    SDL_Texture* voiture_2;
    SDL_Texture* camion;
    SDL_Texture* locomotive;
    SDL_Texture* wagon;
    SDL_Texture* buisson;
    SDL_Texture* arbre;
    SDL_Texture* nenuphare;
    SDL_Texture* rail;
    SDL_Texture* herbe;
    SDL_Texture* route;
    SDL_Texture* eau;
    SDL_Texture* fond;
    SDL_Texture* titre;
} GameTextures;

// -------- Structure pour regrouper les ressources audio -------- // 
typedef struct {
    Mix_Music* backgroundMusic;
    Mix_Chunk* jumpSound;
    Mix_Chunk* deathSound;
} AudioResources;

// -------- Structure pour regrouper les ressources SDL -------- // 
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    GameTextures textures;
    AudioResources audio;
    int cellWidth;
    int cellHeight;
    int isPlayerJumping;
    int playerBounceOffset;
    int playerBounceDirection;
} SDLResources;


// -> Fonctions SDL <- //
void updateGameState(Partie* partie, clock_t* deltaSlow, clock_t* deltaFast);

// -> Jouer une partie graphique <- //
void playPartieSdl(bool isAIControlled, SDLResources* resources);

// -> Menu graphique <- //
void showGraphicalMenu(void);

#endif
