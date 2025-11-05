#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>

#include "renderer.h"
#include "../structures/ligne.h"
#include "../structures/partie.h"
#include "../structures/ia.h"
#include "../helper.h"

// -------- Initalisation -------- // 
// Initialise SDL et crée la fenêtre et le renderer
SDL_bool initSDL(SDLResources* resources, int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init: %s", SDL_GetError());
        return SDL_FALSE;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in image init: %s", SDL_GetError());
        return SDL_FALSE;
    }
    
    if (!initAudio(&resources->audio)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in audio init");
        return SDL_FALSE;    
    }

    resources->window = SDL_CreateWindow("SAULCY ROAD", 
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        SDL_WINDOWPOS_UNDEFINED,
                                        width, height, SDL_WINDOW_SHOWN);
    if (!resources->window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        return SDL_FALSE;
    }

    resources->renderer = SDL_CreateRenderer(resources->window, -1, SDL_RENDERER_ACCELERATED);
    if (!resources->renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        SDL_DestroyWindow(resources->window);
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

// Initialise l'audio avec SDL_mixer
SDL_bool initAudio(AudioResources* audio) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in SDL_mixer init: %s", Mix_GetError());
        return SDL_FALSE;
    }
    
    audio->backgroundMusic = NULL;
    audio->jumpSound = NULL;
    audio->deathSound = NULL;
    
    if (!loadBackgroundMusic(audio)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background music");
        return SDL_FALSE;
    }
    
    if (!loadSoundEffects(audio)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effects");
        return SDL_FALSE;
    }
    
    return SDL_TRUE;
}


// -------- Chargement des ressources -------- //
// Charge la musique de fond
SDL_bool loadBackgroundMusic(AudioResources* audio) {
    audio->backgroundMusic = Mix_LoadMUS("assets/music/background.mp3");
    if (!audio->backgroundMusic) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background music: %s", Mix_GetError());
        return SDL_FALSE;
    }
    return SDL_TRUE;
}

// Charge les effets sonores
SDL_bool loadSoundEffects(AudioResources* audio) {
    audio->jumpSound = Mix_LoadWAV("assets/music/jump.mp3");
    if (!audio->jumpSound) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load jump sound: %s", Mix_GetError());
        return SDL_FALSE;
    }
    
    audio->deathSound = Mix_LoadWAV("assets/music/death.mp3");
    if (!audio->deathSound) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load death sound: %s", Mix_GetError());
        return SDL_FALSE;
    }
    
    return SDL_TRUE;
}

// Charge une texture
SDL_Texture* loadTexture(char* name, SDL_Renderer* renderer, SDL_Window* window) {
    char path[256];
    strcpy(path, "assets/");
    strcat(path, name);
    strcat(path, ".png"); 
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite surface init: %s", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        exit(-1);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        exit(-1);
    }
    return texture;
}

// Charge toutes les textures nécessaires
SDL_bool loadAllTextures(SDLResources* resources) {
    char* personnage = loadCharVariable("file_personnage");
    resources->textures.joueur = loadTexture(personnage, resources->renderer, resources->window);
    resources->textures.buche = loadTexture("buche", resources->renderer, resources->window); 
    resources->textures.voiture_1 = loadTexture("voiture_1", resources->renderer, resources->window);
    resources->textures.voiture_2 = loadTexture("voiture_2", resources->renderer, resources->window);
    resources->textures.camion = loadTexture("camion", resources->renderer, resources->window);
    resources->textures.locomotive = loadTexture("locomotive", resources->renderer, resources->window);
    resources->textures.wagon = loadTexture("wagon", resources->renderer, resources->window);
    resources->textures.buisson = loadTexture("buisson", resources->renderer, resources->window);
    resources->textures.arbre = loadTexture("arbre", resources->renderer, resources->window);
    resources->textures.nenuphare = loadTexture("nenuphare", resources->renderer, resources->window);
    resources->textures.rail = loadTexture("rails", resources->renderer, resources->window);
    resources->textures.herbe = loadTexture("herbe", resources->renderer, resources->window);
    resources->textures.route = loadTexture("route", resources->renderer, resources->window);
    resources->textures.eau = loadTexture("eau", resources->renderer, resources->window);
    resources->textures.fond = loadTexture("fond", resources->renderer, resources->window);
    resources->textures.titre = loadTexture("saulcy", resources->renderer, resources->window);
    
    free(personnage);

    return (resources->textures.joueur && resources->textures.buche && 
            resources->textures.voiture_1 && resources->textures.voiture_2 && 
            resources->textures.camion && resources->textures.locomotive && 
            resources->textures.wagon && resources->textures.buisson && 
            resources->textures.arbre && resources->textures.nenuphare && 
            resources->textures.rail && resources->textures.herbe && 
            resources->textures.route && resources->textures.eau && 
            resources->textures.fond && resources->textures.titre);
}



// -------- Rendu du jeu -------- //
// Dessine le monde de jeu
void renderWorld(SDLResources* resources, Partie* partie, int nbLigne, int nbColonne, int height) {
    SDL_SetRenderDrawColor(resources->renderer, 0, 0, 0, 255);
    SDL_RenderClear(resources->renderer);

    Ligne *currentLigne = getPartieLignes(partie);
    int hideSize = LIMIT_BACKWARD - getJoueurBackwards(getPartieJoueur(partie));
    
    // Avancer aux lignes visibles
    for (int i = 0; i < hideSize && currentLigne != NULL; i++) {
        currentLigne = getLigneNext(currentLigne);
    }
    
    int ligneY = 0;
    
    // Dessiner toutes les lignes visibles
    while (currentLigne != NULL && ligneY < nbLigne) {
        SDL_Rect destRect = {0, height - (ligneY + 1) * resources->cellHeight, nbColonne * resources->cellWidth, resources->cellHeight};
        renderTerrain(resources->renderer, &destRect, getLigneType(currentLigne), &resources->textures);
        
        renderStatiques(resources->renderer, getLigneStatique(currentLigne), ligneY, 
                       resources->cellWidth, resources->cellHeight, nbColonne, &resources->textures, height);
        
        renderDynamiques(resources->renderer, getLigneDynamique(currentLigne), getLigneDirection(currentLigne),
                        ligneY, resources->cellWidth, resources->cellHeight, nbColonne, &resources->textures, height);

        currentLigne = getLigneNext(currentLigne);
        ligneY++;
    }
    renderGameInfo(resources->renderer, partie);
}

// Dessiner les informations du jeu (score)
void renderGameInfo(SDL_Renderer* renderer, Partie* partie) {
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL_ttf: %s", TTF_GetError());
        return;
    }
    
    TTF_Font* font = TTF_OpenFont("assets/fonts/crossy.ttf", 18);
    if (!font) {
        font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 18);
    }
    
    if (font) {
        SDL_Color textColor = {255, 255, 255, 255}; // Blanc
        
        char scoreText[32];
        sprintf(scoreText, "Score %d", getPartieScore(partie));
        
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, textColor);
        
        if (scoreSurface) {
            SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            
            if (scoreTexture) {
                // Définir les positions (en haut à gauche)
                SDL_Rect scoreRect = {10, 10, scoreSurface->w, scoreSurface->h};
                SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
                SDL_DestroyTexture(scoreTexture);
            }
            SDL_FreeSurface(scoreSurface);
        }
        
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

// Dessine les éléments dynamiques
void renderDynamiques(SDL_Renderer* renderer, Dynamique* dynamiqueList, Direction direction,
                     int ligneY, int cellWidth, int cellHeight, int nbColonne, GameTextures* textures, int height) {
    while (dynamiqueList != NULL) {
        int dynX = getDynamiqueCoordX(dynamiqueList) - 4; // Décaler de 4 colonnes pour l'affichage
        int dynLength = getDynamiqueLongueur(dynamiqueList);
        
        // Calculer la position de rendu en fonction de la direction
        int renderX;
        if (direction == GAUCHE) {
            renderX = dynX;
        } else {
            renderX = dynX - (dynLength - 1);
        }
        
        for (int i = 0; i < dynLength; i++) {
            int currentX = renderX + i;
            
            if (currentX >= 0 && currentX < nbColonne) {
                TypeDynamique typeDynamique = getDynamiqueType(dynamiqueList);
                SDL_Texture* textureDynamique = NULL;
                
                switch (typeDynamique) {
                    case VOITURE:
                        textureDynamique = (i % 2 == 0) ? textures->voiture_1 : textures->voiture_2;
                        break;
                    case CAMION:
                        textureDynamique = textures->camion;
                        break;
                    case BUCHE:
                        textureDynamique = textures->buche;
                        break;
                    case TRAIN:
                        // Ajuster pour que la locomotive soit toujours en tête dans la direction du mouvement
                        if (direction == GAUCHE) {
                            // Pour un train se déplaçant vers la gauche, la locomotive est à l'extrémité gauche
                            textureDynamique = (i == 0) ? textures->locomotive : textures->wagon;
                        } else {
                            // Pour un train se déplaçant vers la droite, la locomotive est à l'extrémité droite
                            textureDynamique = (i == dynLength - 1) ? textures->locomotive : textures->wagon;
                        }
                        break;
                }
                
                if (textureDynamique) {
                    SDL_Rect destRect = {
                        currentX * cellWidth, 
                        height - (ligneY + 1) * cellHeight,
                        cellWidth, 
                        cellHeight
                    };
                    
                    // Orientation des sprites selon la direction
                    double angle = 0;
                    if (direction == GAUCHE) angle = 180;
                    
                    SDL_RenderCopyEx(renderer, textureDynamique, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);
                }
            }
        }
        dynamiqueList = getDynamiqueNext(dynamiqueList);
    }
}

// Dessine les éléments statiques
void renderStatiques(SDL_Renderer* renderer, Statique* statiqueList, int ligneY, 
                    int cellWidth, int cellHeight, int nbColonne, GameTextures* textures, int height) {
    while (statiqueList != NULL) {
        int statiqueX = getStatiqueCoordX(statiqueList) - 4; // Décaler de 4 colonnes
        
        if (statiqueX >= 0 && statiqueX < nbColonne) {
            TypeStatique typeStatique = getStatiqueType(statiqueList);
            SDL_Texture* textureStatique = NULL;
            
            switch (typeStatique) {
                case NENUPHARE:
                    textureStatique = textures->nenuphare;
                    break;
                case BUISSON:
                    textureStatique = textures->buisson;
                    break;
                case ARBRE:
                    textureStatique = textures->arbre;
                    break;
            }
            
            if (textureStatique) {
                SDL_Rect destRect = {
                    statiqueX * cellWidth, 
                    height - (ligneY + 1) * cellHeight,
                    cellWidth, 
                    cellHeight
                };
                SDL_RenderCopy(renderer, textureStatique, NULL, &destRect);
            }
        }
        statiqueList = getStatiqueNext(statiqueList);
    }
}

// Dessine un terrain
void renderTerrain(SDL_Renderer* renderer, SDL_Rect* destRect, TypeLigne type, GameTextures* textures) {
    switch (type) {
        case ROUTE:
            SDL_RenderCopy(renderer, textures->route, NULL, destRect);
            break;
        case VOIE_FERREE:
            SDL_RenderCopy(renderer, textures->rail, NULL, destRect);
            break;
        case HERBE:
            SDL_RenderCopy(renderer, textures->herbe, NULL, destRect);
            break;
        case FLEUVE:
        case RAPIDE:
            SDL_RenderCopy(renderer, textures->eau, NULL, destRect);
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, destRect);
            break;
    }
}

// Dessine le joueur
void renderPlayer(SDLResources* resources, Partie* partie, int offset, int ligne, int nbColonne, int height) {
    int joueurX = getJoueurCoordX(getPartieJoueur(partie)) - 4; // Décaler de 4 colonnes
    int joueurYPosition = 4; // Position Y fixe du joueur

    if (joueurX >= 0 && joueurX < nbColonne) {
        int spriteFullWidth, spriteFullHeight;
        SDL_QueryTexture(resources->textures.joueur, NULL, NULL, &spriteFullWidth, &spriteFullHeight);
        
        int spriteWidth = spriteFullWidth / 4;
        int spriteHeight = spriteFullHeight / 5;
        
        SDL_Rect spriteRect = {offset * spriteWidth, ligne * spriteHeight, spriteWidth, spriteHeight};
        
        // Si le joueur est en train de sauter
        if (resources->isPlayerJumping) {
            // Calculer le décalage vertical pour l'effet de rebond
            if (resources->playerBounceDirection == 0) {
                // Monter
                resources->playerBounceOffset += 2;
                if (resources->playerBounceOffset >= 10) {
                    resources->playerBounceDirection = 1;
                }
            } else {
                // Descendre
                resources->playerBounceOffset -= 2;
                if (resources->playerBounceOffset <= 0) {
                    resources->playerBounceOffset = 0;
                    resources->playerBounceDirection = 0;
                    resources->isPlayerJumping = 0;
                }
            }
        }
        
        // Appliquer l'effet de rebond
        SDL_Rect destRect = {
            joueurX * resources->cellWidth, 
            height - (joueurYPosition * resources->cellHeight) - resources->playerBounceOffset, 
            resources->cellWidth, 
            resources->cellHeight
        };
        
        SDL_RenderCopy(resources->renderer, resources->textures.joueur, &spriteRect, &destRect);
    }
}

// Affiche l'overlay de pause avec un texte PAUSE
void renderPauseOverlay(SDL_Renderer* renderer, int width, int height) {
    // Ajoute un filtre semi-transparent
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &overlay);
    
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL_ttf: %s", TTF_GetError());
    } else {
        TTF_Font* font = TTF_OpenFont("assets/fonts/crossy.ttf", 48);
        
        if (!font) {
            font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 48);
        }
        
        if (font) {
            SDL_Color textColor = {255, 255, 255, 255};            
            SDL_Surface* pauseSurface = TTF_RenderText_Solid(font, "PAUSE", textColor);
            
            if (pauseSurface) {
                SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
                
                if (pauseTexture) {
                    SDL_Rect pauseRect = {
                        (width - pauseSurface->w) / 2,
                        (height - pauseSurface->h) / 2,
                        pauseSurface->w,
                        pauseSurface->h
                    };
                    SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);
                    SDL_DestroyTexture(pauseTexture);
                }
                
                SDL_FreeSurface(pauseSurface);
            }
            
            TTF_CloseFont(font);
        }
        
        TTF_Quit();
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}



// -------- Gestion de la musique -------- //
// Joue la musique de fond en boucle
void playBackgroundMusic(AudioResources* audio) {
    if (audio->backgroundMusic) {
        Mix_PlayMusic(audio->backgroundMusic, -1);   // -1 pour jouer en boucle infinie
    }
}

// Joue le son de saut
void playJumpSound(AudioResources* audio) {
    if (audio && audio->jumpSound) {
        Mix_PlayChannel(-1, audio->jumpSound, 0);
    }
}

// Joue le son de mort
void playDeathSound(AudioResources* audio) {
    if (audio && audio->deathSound) {
        Mix_PlayChannel(-1, audio->deathSound, 0);
    }
}

// Met en pause la musique de fond
void pauseBackgroundMusic(void) {
    if (Mix_PlayingMusic()) {
        Mix_PauseMusic();
    }
}

// Reprend la musique de fond
void resumeBackgroundMusic(void) {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

// -------- Gestion des événements -------- //
void handleEvents(Partie* partie, int* running, int* offset, int* ligne, clock_t* deltaIA, clock_t* deltaSlow, clock_t* deltaFast, bool isAIControlled, SDLResources* resources) {
    SDL_Event event;
    // Boucle de gestion des événements SDL
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = 0;
            return;
        }

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                updateEstEnCours(partie); // Inverse l'état de la partie
                if (isPartieEnCours(partie)) {
                    resumeBackgroundMusic();
                    // Réinitialiser les deltas pour éviter des sauts de temps après la reprise
                    *deltaSlow = clock();
                    *deltaFast = clock();
                    *deltaIA = clock();
                } else {
                    pauseBackgroundMusic();
                }
            }
            // Gestion des contrôles du joueur
            else if (!isAIControlled && isPartieEnCours(partie) && isAlive(getPartieJoueur(partie))) {
                bool player_moved = false;
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN:
                    case SDLK_s:
                        *ligne = 0; // Sprite regarde vers le bas
                        if (isCaseAccessible(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)) - 1, getJoueurCoordX(getPartieJoueur(partie)))) {
                            moveJoueur(getPartieJoueur(partie), BAS, false);
                            player_moved = true;
                        }
                        break;
                    case SDLK_UP:
                    case SDLK_z:
                        *ligne = 1; // Sprite regarde vers le haut
                        if (isCaseAccessible(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)) + 1, getJoueurCoordX(getPartieJoueur(partie)))) {
                            if (getJoueurBackwards(getPartieJoueur(partie)) == 0) {
                                partie->ligne = generateNewLigne(getPartieLignes(partie), getPartieJoueur(partie), getPartieScore(partie));
                                addScore(partie);
                            }
                            moveJoueur(getPartieJoueur(partie), HAUT, false);
                            player_moved = true;
                        }
                        break;
                    case SDLK_LEFT:
                    case SDLK_q:
                        *ligne = 2; // Sprite regarde vers la gauche
                        if (isCaseAccessible(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)), getJoueurCoordX(getPartieJoueur(partie)) - 1)) {
                            moveJoueur(getPartieJoueur(partie), GAUCHE, false);
                            player_moved = true;
                        }
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        *ligne = 3; // Sprite regarde vers la droite
                        if (isCaseAccessible(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)), getJoueurCoordX(getPartieJoueur(partie)) + 1)) {
                            moveJoueur(getPartieJoueur(partie), DROITE, false);
                            player_moved = true;
                        }
                        break;
                }

                if (player_moved) {
                    playJumpSound(&resources->audio);
                    *offset = (*offset + 1) % 4;
                    resources->isPlayerJumping = 1;
                    if (isColision(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)), getJoueurCoordX(getPartieJoueur(partie)))) {
                        playDeathSound(&resources->audio);
                        renderDeathAnimation(resources, partie);
                        killJoueur(getPartieJoueur(partie));
                        if(!isAlive(getPartieJoueur(partie))) return; // Sortir si le joueur meurt
                    }
                }
            }
        }
    }

    // Logique de l'IA
    if (isAIControlled && isPartieEnCours(partie) && isAlive(getPartieJoueur(partie))) {
        if ((clock() - *deltaIA) >= (COEF_IA * CLOCKS_PER_SEC) / 8) {
            Direction dir = bestDirectionToGo(getPartieLignes(partie), getPartieJoueur(partie), *deltaSlow);
            bool moved_ia = false;

            switch (dir) {
                case HAUT:
                    *ligne = 1; // Sprite regardant vers le haut
                    if (isCaseAccessible(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)) + 1, getJoueurCoordX(getPartieJoueur(partie)))) {
                        if (getJoueurBackwards(getPartieJoueur(partie)) == 0) {
                            partie->ligne = generateNewLigne(getPartieLignes(partie), getPartieJoueur(partie), getPartieScore(partie));
                            addScore(partie);
                        }
                        moveJoueur(getPartieJoueur(partie), HAUT, false);
                        moved_ia = true;
                    }
                    break;
                case BAS:
                    *ligne = 0; // Sprite regardant vers le bas
                    if (isCaseAccessible(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)) - 1, getJoueurCoordX(getPartieJoueur(partie)))) {
                        moveJoueur(getPartieJoueur(partie), BAS, false);
                        moved_ia = true;
                    }
                    break;
                case GAUCHE:
                    *ligne = 2; // Sprite regardant vers la gauche
                    if (isCaseAccessible(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)), getJoueurCoordX(getPartieJoueur(partie)) - 1)) {
                        moveJoueur(getPartieJoueur(partie), GAUCHE, false);
                        moved_ia = true;
                    }
                    break;
                case DROITE:
                    *ligne = 3; // Sprite regardant vers la droite
                    if (isCaseAccessible(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)), getJoueurCoordX(getPartieJoueur(partie)) + 1)) {
                        moveJoueur(getPartieJoueur(partie), DROITE, false);
                        moved_ia = true;
                    }
                    break;
                default:
                    // L'IA peut choisir de ne pas bouger
                    break;
            }

            if (moved_ia) {
                playJumpSound(&resources->audio);
                *offset = (*offset + 1) % 4;
                resources->isPlayerJumping = 1;
                if (isColision(getPartieLignes(partie), getJoueurCoordY(getPartieJoueur(partie)), getJoueurCoordX(getPartieJoueur(partie)))) {
                    playDeathSound(&resources->audio);
                    renderDeathAnimation(resources, partie);
                    killJoueur(getPartieJoueur(partie));
                }
            }
            *deltaIA = clock();
        }
    }
}

// -------- Annimation de la mort -------- //
void renderDeathAnimation(SDLResources* resources, Partie* partie) {
    playDeathSound(&resources->audio);
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(resources->renderer, &windowWidth, &windowHeight);

    int nbVisibleScreenCols = windowWidth / resources->cellWidth;
    int nbVisibleScreenRows = windowHeight / resources->cellHeight;

    // Coordonnée X et Y actuelle du joueur sur la grille
    int joueurXGrid = getJoueurCoordX(getPartieJoueur(partie)) - 4;
    int joueurScreenX = joueurXGrid * resources->cellWidth;
    int joueurYRenderRowFixed = 4;
    int initialPlayerScreenY = windowHeight - (joueurYRenderRowFixed * resources->cellHeight);

    // Paramètres de l'animation
    int jumpPixelHeight = (int)(resources->cellHeight * 1.5); // Saut de 1.5 cellules de haut
    int animSpeed = 8; // Pixels par image pour le mouvement

    // Recupération de la texture du joueur mort
    int spriteFullWidth, spriteFullHeight;
    SDL_QueryTexture(resources->textures.joueur, NULL, NULL, &spriteFullWidth, &spriteFullHeight);
    int spriteWidth = spriteFullWidth / 4;
    int spriteHeight = spriteFullHeight / 5;
    SDL_Rect spriteRect = {0 * spriteWidth, 4 * spriteHeight, spriteWidth, spriteHeight};

    //Saut vers le haut
    int currentAnimatedY = initialPlayerScreenY;
    for (int yTravelled = 0; yTravelled < jumpPixelHeight; yTravelled += animSpeed) {
        currentAnimatedY -= animSpeed; // Déplacement vers le haut
        renderWorld(resources, partie, nbVisibleScreenRows, nbVisibleScreenCols, windowHeight);
        SDL_Rect destRect = { joueurScreenX, currentAnimatedY, resources->cellWidth, resources->cellHeight };
        SDL_RenderCopy(resources->renderer, resources->textures.joueur, &spriteRect, &destRect);
        renderGameInfo(resources->renderer, partie);
        SDL_RenderPresent(resources->renderer);
        SDL_Delay(16);
    }

    //Chute vers le bas hors de l'écran
    while (currentAnimatedY < windowHeight) { // Tomber jusqu'à être en dessous de l'écran
        currentAnimatedY += animSpeed; // Déplacement vers le bas
        renderWorld(resources, partie, nbVisibleScreenRows, nbVisibleScreenCols, windowHeight);
        SDL_Rect destRect = { joueurScreenX, currentAnimatedY, resources->cellWidth, resources->cellHeight };
        SDL_RenderCopy(resources->renderer, resources->textures.joueur, &spriteRect, &destRect);
        renderGameInfo(resources->renderer, partie);
        SDL_RenderPresent(resources->renderer);
        SDL_Delay(16);
    }
}


// -------- Affichage de la fin de la partie -------- //
SDL_bool showEndScreen(int score, int bestScore, SDLResources* resources) {
    SDL_Renderer *renderer = resources->renderer;
    Mix_PausedMusic();
    
    // Si l'image de fond est chargée, l'afficher, sinon utiliser une couleur de fond noire
    if (resources->textures.fond) {
        SDL_RenderCopy(renderer, resources->textures.fond, NULL, NULL);
        
        // Ajouter un effet de flou en appliquant un filtre semi-transparent
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // Noir semi-transparent pour créer l'effet de flou
        
        int windowWidth, windowHeight;
        SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
        
        SDL_Rect blurRect = {0, 0, windowWidth, windowHeight};
        SDL_RenderFillRect(renderer, &blurRect);
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    
    // Récupérer les dimensions de la fenêtre
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    
    // Afficher le titre du jeu en haut de l'écran
    if (resources->textures.titre) {
        int titreWidth, titreHeight;
        SDL_QueryTexture(resources->textures.titre, NULL, NULL, &titreWidth, &titreHeight);
        
        // Calculer la position pour centrer le titre en haut
        // On le place au-dessus du "GAME OVER"
        SDL_Rect titreRect = {
            (windowWidth - titreWidth) / 2,
            windowHeight / 8 - titreHeight / 2,
            titreWidth,
            titreHeight
        };
        
        SDL_RenderCopy(renderer, resources->textures.titre, NULL, &titreRect);
    }
    
    SDL_bool playAgain = SDL_FALSE;
    
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL_ttf: %s", TTF_GetError());
    } else {
        TTF_Font* font = TTF_OpenFont("assets/fonts/crossy.ttf", 24);
        TTF_Font* buttonFont = TTF_OpenFont("assets/fonts/crossy.ttf", 16);
        
        if (!font) {
            font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 28);
            buttonFont = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 24);
        }
        
        if (font && buttonFont) {
            SDL_Color textColor = {255, 255, 255, 255}; // Blanc
            SDL_Color buttonTextColor = {0, 0, 0, 255}; // Noir
            
            // Créer le titre "Game Over"
            SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "GAME OVER", textColor);
            
            // Créer les surfaces de texte pour les scores
            SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, "Score final", textColor);
            char scoreText[10];
            sprintf(scoreText, "%d", score);
            SDL_Surface* scoreValueSurface = TTF_RenderText_Solid(font, scoreText, textColor);

            SDL_Surface* bestScoreSurface = TTF_RenderText_Solid(font, "Best score", textColor);
            char bestScoreText[10];
            sprintf(bestScoreText, "%d", bestScore);
            SDL_Surface* bestScoreValueSurface = TTF_RenderText_Solid(font, bestScoreText, textColor);
            
            // Créer les surfaces pour les boutons
            SDL_Surface* replayButtonSurface = TTF_RenderText_Solid(buttonFont, "Rejouer", buttonTextColor);
            SDL_Surface* menuButtonSurface = TTF_RenderText_Solid(buttonFont, "Menu", buttonTextColor);
            
            // Convertir les surfaces en textures
            if (titleSurface && scoreSurface && scoreValueSurface && bestScoreSurface && bestScoreValueSurface &&
                replayButtonSurface && menuButtonSurface) {
                
                // Créer les textures
                SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
                SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
                SDL_Texture* scoreValueTexture = SDL_CreateTextureFromSurface(renderer, scoreValueSurface);
                SDL_Texture* bestScoreTexture = SDL_CreateTextureFromSurface(renderer, bestScoreSurface);
                SDL_Texture* bestScoreValueTexture = SDL_CreateTextureFromSurface(renderer, bestScoreValueSurface);
                SDL_Texture* replayButtonTexture = SDL_CreateTextureFromSurface(renderer, replayButtonSurface);
                SDL_Texture* menuButtonTexture = SDL_CreateTextureFromSurface(renderer, menuButtonSurface);
                
                // Positionner le titre "GAME OVER"
                // Ajuster la position pour qu'il soit en dessous du logo Saulcy
                SDL_Rect titleRect = {(windowWidth - titleSurface->w) / 2, windowHeight / 4, 
                                     titleSurface->w, titleSurface->h};
                
                // Positionner les textes des scores
                int centerX = windowWidth / 2;
                SDL_Rect scoreRect = {centerX-110, windowHeight / 2 - 70, scoreSurface->w, scoreSurface->h};
                SDL_Rect scoreValueRect = {centerX, windowHeight / 2 - 30, scoreValueSurface->w, scoreValueSurface->h};
                
                SDL_Rect bestScoreRect = {centerX-105, windowHeight / 2 + 30, bestScoreSurface->w, bestScoreSurface->h};
                SDL_Rect bestScoreValueRect = {centerX-5, windowHeight / 2 + 70, bestScoreValueSurface->w, bestScoreValueSurface->h};
                
                // Définir les rectangles pour les boutons (avec bordure colorée)
                SDL_Rect replayButtonBg = {centerX - 150, windowHeight * 3/4, 120, 50};
                SDL_Rect menuButtonBg = {centerX + 30, windowHeight * 3/4, 120, 50};
                
                SDL_Rect replayButtonRect = {replayButtonBg.x + (replayButtonBg.w - replayButtonSurface->w) / 2, 
                                           replayButtonBg.y + (replayButtonBg.h - replayButtonSurface->h) / 2,
                                           replayButtonSurface->w, replayButtonSurface->h};
                                           
                SDL_Rect menuButtonRect = {menuButtonBg.x + (menuButtonBg.w - menuButtonSurface->w) / 2,
                                         menuButtonBg.y + (menuButtonBg.h - menuButtonSurface->h) / 2,
                                         menuButtonSurface->w, menuButtonSurface->h};
                
                // Dessiner les éléments
                SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
                SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
                SDL_RenderCopy(renderer, scoreValueTexture, NULL, &scoreValueRect);
                SDL_RenderCopy(renderer, bestScoreTexture, NULL, &bestScoreRect);
                SDL_RenderCopy(renderer, bestScoreValueTexture, NULL, &bestScoreValueRect);
                
                // Dessiner les fonds des boutons
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Gris clair
                SDL_RenderFillRect(renderer, &replayButtonBg);
                SDL_RenderFillRect(renderer, &menuButtonBg);
                
                // Ajouter une bordure aux boutons
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Gris foncé
                SDL_RenderDrawRect(renderer, &replayButtonBg);
                SDL_RenderDrawRect(renderer, &menuButtonBg);
                
                // Dessiner le texte des boutons
                SDL_RenderCopy(renderer, replayButtonTexture, NULL, &replayButtonRect);
                SDL_RenderCopy(renderer, menuButtonTexture, NULL, &menuButtonRect);
                
                // Rafraîchir l'affichage
                SDL_RenderPresent(renderer);
                
                // Attendre un clic de l'utilisateur sur l'un des boutons
                SDL_Event endScreenEvent;
                SDL_bool waitingForChoice = SDL_TRUE;
                
                while (waitingForChoice) {
                    while (SDL_PollEvent(&endScreenEvent)) {
                        if (endScreenEvent.type == SDL_QUIT) {
                            waitingForChoice = SDL_FALSE;
                            break;
                        }
                        
                        if (endScreenEvent.type == SDL_MOUSEBUTTONDOWN) {
                            int mouseX = endScreenEvent.button.x;
                            int mouseY = endScreenEvent.button.y;
                            
                            // Vérifier si le clic est sur le bouton "Rejouer"
                            if (mouseX >= replayButtonBg.x && mouseX <= replayButtonBg.x + replayButtonBg.w &&
                                mouseY >= replayButtonBg.y && mouseY <= replayButtonBg.y + replayButtonBg.h) {
                                playAgain = SDL_TRUE;
                                waitingForChoice = SDL_FALSE;
                            }
                            // Vérifier si le clic est sur le bouton "Menu"
                            else if (mouseX >= menuButtonBg.x && mouseX <= menuButtonBg.x + menuButtonBg.w &&
                                     mouseY >= menuButtonBg.y && mouseY <= menuButtonBg.y + menuButtonBg.h) {
                                playAgain = SDL_FALSE;
                                waitingForChoice = SDL_FALSE;
                            }
                        }
                        
                        // Gestion des touches clavier comme fallback
                        if (endScreenEvent.type == SDL_KEYDOWN) {
                            if (endScreenEvent.key.keysym.sym == SDLK_y || 
                                endScreenEvent.key.keysym.sym == SDLK_o || 
                                endScreenEvent.key.keysym.sym == SDLK_RETURN) {
                                playAgain = SDL_TRUE;
                                waitingForChoice = SDL_FALSE;
                            } else if (endScreenEvent.key.keysym.sym == SDLK_n || 
                                       endScreenEvent.key.keysym.sym == SDLK_ESCAPE) {
                                waitingForChoice = SDL_FALSE;
                            }
                        }
                    }
                    
                    SDL_Delay(10); // Petite pause pour éviter de surcharger le CPU
                }
                
                // Libération des textures
                SDL_DestroyTexture(titleTexture);
                SDL_DestroyTexture(scoreTexture);
                SDL_DestroyTexture(scoreValueTexture);
                SDL_DestroyTexture(bestScoreTexture);
                SDL_DestroyTexture(bestScoreValueTexture);
                SDL_DestroyTexture(replayButtonTexture);
                SDL_DestroyTexture(menuButtonTexture);
            }
            
            // Libération des surfaces
            if (titleSurface) SDL_FreeSurface(titleSurface);
            if (scoreSurface) SDL_FreeSurface(scoreSurface);
            if (scoreValueSurface) SDL_FreeSurface(scoreValueSurface);
            if (bestScoreSurface) SDL_FreeSurface(bestScoreSurface);
            if (bestScoreValueSurface) SDL_FreeSurface(bestScoreValueSurface);
            if (replayButtonSurface) SDL_FreeSurface(replayButtonSurface);
            if (menuButtonSurface) SDL_FreeSurface(menuButtonSurface);
        }
        
        // Fermeture des polices
        if (font) TTF_CloseFont(font);
        if (buttonFont) TTF_CloseFont(buttonFont);
        
        TTF_Quit();
    }
    
    return playAgain;
}


// Vestiaire
void showWardrobe(SDLResources* resources) {
    SDL_Renderer *renderer = resources->renderer;
    
    // AJOUTER ICI LES SKINS DISPONIBLES
    char* availableSkins[] = {
        "personnage/poulet",
        "personnage/croco",
        "personnage/canard"
    };
    
    int numSkins = sizeof(availableSkins) / sizeof(availableSkins[0]);
    

    char* currentSkin = loadCharVariable("file_personnage");
    int selectedIndex = 0;
    

    for (int i = 0; i < numSkins; i++) {
        if (currentSkin && strcmp(currentSkin, availableSkins[i]) == 0) {
            selectedIndex = i;
            break;
        }
    }
    if (currentSkin) free(currentSkin);
    

    SDL_Texture* skinTextures[numSkins];
    for (int i = 0; i < numSkins; i++) {
        skinTextures[i] = loadTexture(availableSkins[i], renderer, resources->window);
    }
    
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    

    SDL_Rect titreRect = {0, 0, 0, 0};
    

    if (resources->textures.titre) {
        int titreWidth, titreHeight;
        SDL_QueryTexture(resources->textures.titre, NULL, NULL, &titreWidth, &titreHeight);
        
        titreRect.x = (windowWidth - titreWidth) / 2;
        titreRect.y = windowHeight / 8;
        titreRect.w = titreWidth;
        titreRect.h = titreHeight;
    }
    
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL_ttf: %s", TTF_GetError());
        return;
    }
    
    TTF_Font* buttonFont = TTF_OpenFont("assets/fonts/crossy.ttf", 20);
    
    if (!buttonFont) {
        buttonFont = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 20);
    }
    
    // Police spécifique pour les flèches
    TTF_Font* arrowFont = TTF_OpenFont("assets/fonts/arial.ttf", 20);
    
    if (buttonFont && arrowFont) {
        SDL_Color buttonTextColor = {0, 0, 0, 255}; 
        

        SDL_Surface* selectButtonSurface = TTF_RenderText_Solid(buttonFont, "Selectionner", buttonTextColor);
        SDL_Surface* leftArrowSurface = TTF_RenderText_Solid(arrowFont, "<", buttonTextColor);
        SDL_Surface* rightArrowSurface = TTF_RenderText_Solid(arrowFont, ">", buttonTextColor);
        
        if (selectButtonSurface && leftArrowSurface && rightArrowSurface) {
            SDL_Texture* selectButtonTexture = SDL_CreateTextureFromSurface(renderer, selectButtonSurface);
            SDL_Texture* leftArrowTexture = SDL_CreateTextureFromSurface(renderer, leftArrowSurface);
            SDL_Texture* rightArrowTexture = SDL_CreateTextureFromSurface(renderer, rightArrowSurface);
            
            int skinDisplaySize = 120;
            SDL_Rect skinDisplayRect = {
                (windowWidth - skinDisplaySize) / 2,
                windowHeight / 2 - skinDisplaySize / 2,
                skinDisplaySize,
                skinDisplaySize
            };  
            
            // Boutons de navigation
            int arrowSize = 40;
            SDL_Rect leftArrowBg = {
                skinDisplayRect.x - 80,
                skinDisplayRect.y + skinDisplaySize / 2 - arrowSize / 2,
                arrowSize,
                arrowSize
            };
            
            SDL_Rect rightArrowBg = {
                skinDisplayRect.x + skinDisplaySize + 40,
                skinDisplayRect.y + skinDisplaySize / 2 - arrowSize / 2,
                arrowSize,
                arrowSize
            };
            
            int padding = 20;
            SDL_Rect selectButtonBg = {
                (windowWidth - selectButtonSurface->w - padding) / 2,
                windowHeight * 3/4,
                selectButtonSurface->w + padding,
                selectButtonSurface->h + padding
            };
            

            SDL_Rect selectButtonRect = {
                selectButtonBg.x + padding / 2,
                selectButtonBg.y + padding / 2,
                selectButtonSurface->w, 
                selectButtonSurface->h
            };
            
            SDL_Rect leftArrowRect = {
                leftArrowBg.x + (leftArrowBg.w - leftArrowSurface->w) / 2,
                leftArrowBg.y + (leftArrowBg.h - leftArrowSurface->h) / 2,
                leftArrowSurface->w, leftArrowSurface->h
            };
            
            SDL_Rect rightArrowRect = {
                rightArrowBg.x + (rightArrowBg.w - rightArrowSurface->w) / 2,
                rightArrowBg.y + (rightArrowBg.h - rightArrowSurface->h) / 2,
                rightArrowSurface->w, rightArrowSurface->h
            };
            
            SDL_Event wardrobeEvent;
            SDL_bool inWardrobe = SDL_TRUE;
            
            while (inWardrobe) {
                while (SDL_PollEvent(&wardrobeEvent)) {
                    if (wardrobeEvent.type == SDL_QUIT) {
                        inWardrobe = SDL_FALSE;
                        break;
                    }
                    
                    if (wardrobeEvent.type == SDL_MOUSEBUTTONDOWN) {
                        int mouseX = wardrobeEvent.button.x;
                        int mouseY = wardrobeEvent.button.y;
                        

                        if (mouseX >= leftArrowBg.x && mouseX <= leftArrowBg.x + leftArrowBg.w &&
                            mouseY >= leftArrowBg.y && mouseY <= leftArrowBg.y + leftArrowBg.h) {
                            selectedIndex = (selectedIndex - 1 + numSkins) % numSkins;
                        }
                        else if (mouseX >= rightArrowBg.x && mouseX <= rightArrowBg.x + rightArrowBg.w &&
                                 mouseY >= rightArrowBg.y && mouseY <= rightArrowBg.y + rightArrowBg.h) {
                            selectedIndex = (selectedIndex + 1) % numSkins;
                        }
                        else if (mouseX >= selectButtonBg.x && mouseX <= selectButtonBg.x + selectButtonBg.w &&
                                 mouseY >= selectButtonBg.y && mouseY <= selectButtonBg.y + selectButtonBg.h) {

                            saveCharVariable(availableSkins[selectedIndex], "file_personnage");
                            inWardrobe = SDL_FALSE;
                        }
                    }
                    
                    if (wardrobeEvent.type == SDL_KEYDOWN) {
                        switch (wardrobeEvent.key.keysym.sym) {
                            case SDLK_LEFT:
                            case SDLK_q:
                                selectedIndex = (selectedIndex - 1 + numSkins) % numSkins;
                                break;
                            case SDLK_RIGHT:
                            case SDLK_d:
                                selectedIndex = (selectedIndex + 1) % numSkins;
                                break;
                            case SDLK_RETURN:
                            case SDLK_SPACE:
                                saveCharVariable(availableSkins[selectedIndex], "file_personnage");
                                inWardrobe = SDL_FALSE;
                                break;
                            case SDLK_ESCAPE:
                                inWardrobe = SDL_FALSE;
                                break;
                        }
                    }
                }
                
                if (resources->textures.fond) {
                    SDL_RenderCopy(renderer, resources->textures.fond, NULL, NULL);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                }
                
                if (resources->textures.titre) {
                    SDL_RenderCopy(renderer, resources->textures.titre, NULL, &titreRect);
                }
                
                if (skinTextures[selectedIndex]) {
                    int spriteFullWidth, spriteFullHeight;
                    SDL_QueryTexture(skinTextures[selectedIndex], NULL, NULL, &spriteFullWidth, &spriteFullHeight);
                    int spriteWidth = spriteFullWidth / 4;
                    int spriteHeight = spriteFullHeight / 5;
                    
                    SDL_Rect spriteRect = {0, 0, spriteWidth, spriteHeight};
                    SDL_RenderCopy(renderer, skinTextures[selectedIndex], &spriteRect, &skinDisplayRect);
                }
                
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); 
                SDL_RenderFillRect(renderer, &leftArrowBg);
                SDL_RenderFillRect(renderer, &rightArrowBg);
                
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderDrawRect(renderer, &leftArrowBg);
                SDL_RenderDrawRect(renderer, &rightArrowBg);
                
                SDL_RenderCopy(renderer, leftArrowTexture, NULL, &leftArrowRect);
                SDL_RenderCopy(renderer, rightArrowTexture, NULL, &rightArrowRect);
                

                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                SDL_RenderFillRect(renderer, &selectButtonBg);
                
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderDrawRect(renderer, &selectButtonBg);
                
                SDL_RenderCopy(renderer, selectButtonTexture, NULL, &selectButtonRect);
                
                SDL_RenderPresent(renderer);
                SDL_Delay(16);
            }
            
            if (selectButtonTexture) SDL_DestroyTexture(selectButtonTexture);
            if (leftArrowTexture) SDL_DestroyTexture(leftArrowTexture);
            if (rightArrowTexture) SDL_DestroyTexture(rightArrowTexture);
        }
        
        if (selectButtonSurface) SDL_FreeSurface(selectButtonSurface);
        if (leftArrowSurface) SDL_FreeSurface(leftArrowSurface);
        if (rightArrowSurface) SDL_FreeSurface(rightArrowSurface);
    }
    
    for (int i = 0; i < numSkins; i++) {
        if (skinTextures[i]) SDL_DestroyTexture(skinTextures[i]);
    }
    
    if (buttonFont) TTF_CloseFont(buttonFont);
    if (arrowFont) TTF_CloseFont(arrowFont);
    
    TTF_Quit();
}

// Menu graphique 
int showMainMenu(SDLResources* resources) {
    SDL_Renderer *renderer = resources->renderer;
    
    int menuChoice = -1;
    
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL_ttf: %s", TTF_GetError());
        return 0;
    }
    
    TTF_Font* buttonFont = TTF_OpenFont("assets/fonts/crossy.ttf", 20);
    
    if (!buttonFont) {
        buttonFont = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 20);
    }
    
    if (buttonFont) {
        SDL_Color buttonTextColor = {0, 0, 0, 255}; // Noir
        

        SDL_Surface* playerButtonSurface = TTF_RenderText_Solid(buttonFont, "Jouer", buttonTextColor);
        SDL_Surface* iaButtonSurface = TTF_RenderText_Solid(buttonFont, "Mode IA", buttonTextColor);
        SDL_Surface* wardrobeButtonSurface = TTF_RenderText_Solid(buttonFont, "Vestiaire", buttonTextColor);
        SDL_Surface* quitButtonSurface = TTF_RenderText_Solid(buttonFont, "Quitter", buttonTextColor);
        
        if (playerButtonSurface && iaButtonSurface && wardrobeButtonSurface && quitButtonSurface) {
            SDL_Texture* playerButtonTexture = SDL_CreateTextureFromSurface(renderer, playerButtonSurface);
            SDL_Texture* iaButtonTexture = SDL_CreateTextureFromSurface(renderer, iaButtonSurface);
            SDL_Texture* wardrobeButtonTexture = SDL_CreateTextureFromSurface(renderer, wardrobeButtonSurface);
            SDL_Texture* quitButtonTexture = SDL_CreateTextureFromSurface(renderer, quitButtonSurface);
            

            int windowWidth, windowHeight;
            SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
            
            int buttonWidth = 200;
            int buttonHeight = 50;
            int buttonSpacing = 15;
            int centerX = windowWidth / 2;
            int startY = windowHeight / 2 - 40;
            
            SDL_Rect playerButtonBg = {centerX - buttonWidth/2, startY, buttonWidth, buttonHeight};
            SDL_Rect iaButtonBg = {centerX - buttonWidth/2, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight};
            SDL_Rect wardrobeButtonBg = {centerX - buttonWidth/2, startY + 2*(buttonHeight + buttonSpacing), buttonWidth, buttonHeight};
            SDL_Rect quitButtonBg = {centerX - buttonWidth/2, startY + 3*(buttonHeight + buttonSpacing), buttonWidth, buttonHeight};
            

            SDL_Rect playerButtonRect = {
                playerButtonBg.x + (playerButtonBg.w - playerButtonSurface->w) / 2,
                playerButtonBg.y + (playerButtonBg.h - playerButtonSurface->h) / 2,
                playerButtonSurface->w, playerButtonSurface->h
            };
            
            SDL_Rect iaButtonRect = {
                iaButtonBg.x + (iaButtonBg.w - iaButtonSurface->w) / 2,
                iaButtonBg.y + (iaButtonBg.h - iaButtonSurface->h) / 2,
                iaButtonSurface->w, iaButtonSurface->h
            };
            
            SDL_Rect wardrobeButtonRect = {
                wardrobeButtonBg.x + (wardrobeButtonBg.w - wardrobeButtonSurface->w) / 2,
                wardrobeButtonBg.y + (wardrobeButtonBg.h - wardrobeButtonSurface->h) / 2,
                wardrobeButtonSurface->w, wardrobeButtonSurface->h
            };
            
            SDL_Rect quitButtonRect = {
                quitButtonBg.x + (quitButtonBg.w - quitButtonSurface->w) / 2,
                quitButtonBg.y + (quitButtonBg.h - quitButtonSurface->h) / 2,
                quitButtonSurface->w, quitButtonSurface->h
            };
            
            SDL_Event menuEvent;
            SDL_bool waitingForChoice = SDL_TRUE;
            
            while (waitingForChoice) {
                while (SDL_PollEvent(&menuEvent)) {
                    if (menuEvent.type == SDL_QUIT) {
                        menuChoice = 0; // Quitter
                        waitingForChoice = SDL_FALSE;
                        break;
                    }
                    
                    if (menuEvent.type == SDL_MOUSEBUTTONDOWN) {
                        int mouseX = menuEvent.button.x;
                        int mouseY = menuEvent.button.y;
                        

                        if (mouseX >= playerButtonBg.x && mouseX <= playerButtonBg.x + playerButtonBg.w &&
                            mouseY >= playerButtonBg.y && mouseY <= playerButtonBg.y + playerButtonBg.h) {
                            menuChoice = 1; // Jouer
                            waitingForChoice = SDL_FALSE;
                        }
                        else if (mouseX >= iaButtonBg.x && mouseX <= iaButtonBg.x + iaButtonBg.w &&
                                 mouseY >= iaButtonBg.y && mouseY <= iaButtonBg.y + iaButtonBg.h) {
                            menuChoice = 2; // Mode IA
                            waitingForChoice = SDL_FALSE;
                        }
                        else if (mouseX >= wardrobeButtonBg.x && mouseX <= wardrobeButtonBg.x + wardrobeButtonBg.w &&
                                 mouseY >= wardrobeButtonBg.y && mouseY <= wardrobeButtonBg.y + wardrobeButtonBg.h) {
                            menuChoice = 3; // Vestiaire
                            waitingForChoice = SDL_FALSE;
                        }
                        else if (mouseX >= quitButtonBg.x && mouseX <= quitButtonBg.x + quitButtonBg.w &&
                                 mouseY >= quitButtonBg.y && mouseY <= quitButtonBg.y + quitButtonBg.h) {
                            menuChoice = 0; // Quitter
                            waitingForChoice = SDL_FALSE;
                        }
                    }
                    

                    if (menuEvent.type == SDL_KEYDOWN) {
                        if (menuEvent.key.keysym.sym == SDLK_1) {
                            menuChoice = 1;
                            waitingForChoice = SDL_FALSE;
                        } else if (menuEvent.key.keysym.sym == SDLK_2) {
                            menuChoice = 2;
                            waitingForChoice = SDL_FALSE;
                        } else if (menuEvent.key.keysym.sym == SDLK_3) {
                            menuChoice = 3;
                            waitingForChoice = SDL_FALSE;
                        } else if (menuEvent.key.keysym.sym == SDLK_ESCAPE || menuEvent.key.keysym.sym == SDLK_q) {
                            menuChoice = 0;
                            waitingForChoice = SDL_FALSE;
                        }
                    }
                }
                

                if (resources->textures.fond) {
                    SDL_RenderCopy(renderer, resources->textures.fond, NULL, NULL);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                }
                

                if (resources->textures.titre) {
                    int titreWidth, titreHeight;
                    SDL_QueryTexture(resources->textures.titre, NULL, NULL, &titreWidth, &titreHeight);
                    
                    SDL_Rect titreRect = {
                        (windowWidth - titreWidth) / 2,
                        windowHeight / 6,
                        titreWidth,
                        titreHeight
                    };
                    
                    SDL_RenderCopy(renderer, resources->textures.titre, NULL, &titreRect);
                }
                

                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                SDL_RenderFillRect(renderer, &playerButtonBg);
                SDL_RenderFillRect(renderer, &iaButtonBg);
                SDL_RenderFillRect(renderer, &wardrobeButtonBg);
                SDL_RenderFillRect(renderer, &quitButtonBg);
                

                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); 
                SDL_RenderDrawRect(renderer, &playerButtonBg);
                SDL_RenderDrawRect(renderer, &iaButtonBg);
                SDL_RenderDrawRect(renderer, &wardrobeButtonBg);
                SDL_RenderDrawRect(renderer, &quitButtonBg);
                
                SDL_RenderCopy(renderer, playerButtonTexture, NULL, &playerButtonRect);
                SDL_RenderCopy(renderer, iaButtonTexture, NULL, &iaButtonRect);
                SDL_RenderCopy(renderer, wardrobeButtonTexture, NULL, &wardrobeButtonRect);
                SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButtonRect);
                
                SDL_RenderPresent(renderer);
                SDL_Delay(16);
            }
            
            if (playerButtonTexture) SDL_DestroyTexture(playerButtonTexture);
            if (iaButtonTexture) SDL_DestroyTexture(iaButtonTexture);
            if (wardrobeButtonTexture) SDL_DestroyTexture(wardrobeButtonTexture);
            if (quitButtonTexture) SDL_DestroyTexture(quitButtonTexture);
        }
        
        if (playerButtonSurface) SDL_FreeSurface(playerButtonSurface);
        if (iaButtonSurface) SDL_FreeSurface(iaButtonSurface);
        if (wardrobeButtonSurface) SDL_FreeSurface(wardrobeButtonSurface);
        if (quitButtonSurface) SDL_FreeSurface(quitButtonSurface);
    }
    
    if (buttonFont) TTF_CloseFont(buttonFont);
    
    TTF_Quit();
    
    return menuChoice;
}

// -------- Libération des ressources -------- //
// Libère toutes les textures
void freeAllTextures(GameTextures* textures) {
    SDL_DestroyTexture(textures->locomotive);
    SDL_DestroyTexture(textures->wagon);
    SDL_DestroyTexture(textures->joueur);
    SDL_DestroyTexture(textures->buche);
    SDL_DestroyTexture(textures->nenuphare);
    SDL_DestroyTexture(textures->voiture_1);
    SDL_DestroyTexture(textures->voiture_2);
    SDL_DestroyTexture(textures->camion);
    SDL_DestroyTexture(textures->buisson);
    SDL_DestroyTexture(textures->arbre);
    SDL_DestroyTexture(textures->rail);
    SDL_DestroyTexture(textures->herbe);
    SDL_DestroyTexture(textures->route);
    SDL_DestroyTexture(textures->eau);
    SDL_DestroyTexture(textures->fond);
    SDL_DestroyTexture(textures->titre);
}

// Suppression ressources SDL audio
void cleanupAudio(AudioResources* audio) {
    if (audio->backgroundMusic) {
        Mix_FreeMusic(audio->backgroundMusic);
        audio->backgroundMusic = NULL;
    }
    
    if (audio->jumpSound) {
        Mix_FreeChunk(audio->jumpSound);
        audio->jumpSound = NULL;
    }
    
    if (audio->deathSound) {
        Mix_FreeChunk(audio->deathSound);
        audio->deathSound = NULL;
    }
    
    Mix_CloseAudio();
}

// Libère les ressources SDL
void cleanupSDL(SDLResources* resources) {
    freeAllTextures(&resources->textures);
    cleanupAudio(&resources->audio);
    SDL_DestroyRenderer(resources->renderer);
    SDL_DestroyWindow(resources->window);
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

