#ifndef RENDERER
#define RENDERER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>

#include "../structures/partie.h"
#include "../structures/ia.h"

// -------- Initalisation -------- // 
SDL_bool initSDL(SDLResources* resource, int width, int height);
SDL_bool initAudio(AudioResources* audio);

// -------- Chargement des ressources -------- //
SDL_Texture* loadTexture(char* name, SDL_Renderer* renderer, SDL_Window* window);
SDL_bool loadAllTextures(SDLResources* resources);
SDL_bool loadBackgroundMusic(AudioResources* audio);
SDL_bool loadSoundEffects(AudioResources* audio);

// -------- Rendu du jeu -------- //
void renderWorld(SDLResources* resources, Partie* partie, int nbLigne, int nbColonne, int height);
void renderDynamiques(SDL_Renderer* renderer, Dynamique* dynamiqueList, Direction direction,
                    int ligneY, int cellWidth, int cellHeight, int nbColonne, GameTextures* textures, int height);

void renderStatiques(SDL_Renderer* renderer, Statique* statiqueList, int ligneY, 
                    int cellWidth, int cellHeight, int nbColonne, GameTextures* textures, int height);
            
void renderTerrain(SDL_Renderer* renderer, SDL_Rect* destRect, TypeLigne type, GameTextures* textures);
void renderPlayer(SDLResources* resources, Partie* partie, int offset, int ligne, int nbColonne, int height);
void renderPauseOverlay(SDL_Renderer* renderer, int width, int height);
void renderGameInfo(SDL_Renderer* renderer, Partie* partie);
void renderDeathAnimation(SDLResources* resources, Partie* partie);

// -------- Gestion de la musique -------- //
void playBackgroundMusic(AudioResources* audio);
void pauseBackgroundMusic(void);
void resumeBackgroundMusic(void);
void playJumpSound(AudioResources* audio);
void playDeathSound(AudioResources* audio);

// -------- Gestion des événements -------- //
void handleEvents(Partie* partie, int* running, int* offset, int* ligne, clock_t* deltaIA, clock_t* deltaSlow, clock_t* deltaFast, bool isAIControlled, SDLResources* resources);


// -------- Affichage de la fin de la partie -------- //
SDL_bool showEndScreen(int score, int bestScore, SDLResources* resources);

// -------- Menu principal graphique -------- //
int showMainMenu(SDLResources* resources);

// -------- Vestiaire -------- //
void showWardrobe(SDLResources* resources);

// -------- Libération des ressources -------- //
void freeAllTextures(GameTextures* textures);
void cleanupAudio(AudioResources* audio);
void cleanupSDL(SDLResources* resources);

#endif
