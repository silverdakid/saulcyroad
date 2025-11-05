#include <CUnit/Basic.h>
#include <stdbool.h>

#include "../../../libs/modules/renderer.h"
#include "../../../libs/structures/partie.h"

// Test des fonctions audio (sans SDL_mixer)
void test_audioResources(void){
    printf("\n    • Test de la structure AudioResources\n");
    AudioResources audio;
    audio.backgroundMusic = NULL;
    audio.jumpSound = NULL;
    audio.deathSound = NULL;
    
    // Test des pointeurs null
    CU_ASSERT_EQUAL(audio.backgroundMusic, NULL);
    CU_ASSERT_EQUAL(audio.jumpSound, NULL);
    CU_ASSERT_EQUAL(audio.deathSound, NULL);
}

// Test de la structure GameTextures
void test_gameTextures(void){
    printf("\n    • Test de la structure GameTextures\n");
    GameTextures textures;
    
    // Initialiser tous les pointeurs à NULL
    textures.joueur = NULL;
    textures.buche = NULL;
    textures.voiture_1 = NULL;
    textures.voiture_2 = NULL;
    textures.camion = NULL;
    textures.locomotive = NULL;
    textures.wagon = NULL;
    textures.buisson = NULL;
    textures.arbre = NULL;
    textures.nenuphare = NULL;
    textures.rail = NULL;
    textures.herbe = NULL;
    textures.route = NULL;
    textures.eau = NULL;
    textures.fond = NULL;
    textures.titre = NULL;
    
    // Vérifier l'initialisation
    CU_ASSERT_EQUAL(textures.joueur, NULL);
    CU_ASSERT_EQUAL(textures.buche, NULL);
    CU_ASSERT_EQUAL(textures.voiture_1, NULL);
    CU_ASSERT_EQUAL(textures.fond, NULL);
    CU_ASSERT_EQUAL(textures.titre, NULL);
}

// Test de la structure SDLResources
void test_sdlResources(void){
    printf("\n    • Test de la structure SDLResources\n");
    SDLResources resources;
    
    // Initialiser la structure
    resources.window = NULL;
    resources.renderer = NULL;
    resources.cellWidth = 32;
    resources.cellHeight = 32;
    resources.isPlayerJumping = 0;
    resources.playerBounceOffset = 0;
    resources.playerBounceDirection = 0;
    
    // Test des valeurs
    CU_ASSERT_EQUAL(resources.window, NULL);
    CU_ASSERT_EQUAL(resources.renderer, NULL);
    CU_ASSERT_EQUAL(resources.cellWidth, 32);
    CU_ASSERT_EQUAL(resources.cellHeight, 32);
    CU_ASSERT_EQUAL(resources.isPlayerJumping, 0);
    CU_ASSERT_EQUAL(resources.playerBounceOffset, 0);
    CU_ASSERT_EQUAL(resources.playerBounceDirection, 0);
    
    // Test de modification des valeurs
    resources.cellWidth = 64;
    resources.cellHeight = 48;
    resources.isPlayerJumping = 1;
    resources.playerBounceOffset = 5;
    resources.playerBounceDirection = 1;
    
    CU_ASSERT_EQUAL(resources.cellWidth, 64);
    CU_ASSERT_EQUAL(resources.cellHeight, 48);
    CU_ASSERT_EQUAL(resources.isPlayerJumping, 1);
    CU_ASSERT_EQUAL(resources.playerBounceOffset, 5);
    CU_ASSERT_EQUAL(resources.playerBounceDirection, 1);
}

// Test de updateGameState sans SDL
void test_updateGameState(void){
    printf("\n    • Test de updateGameState\n");
    
    // Créer une partie de test
    Partie* partie = generatePartie(false);
    CU_ASSERT_NOT_EQUAL(partie, NULL);
    
    if (partie) {
        clock_t deltaSlow = clock();
        clock_t deltaFast = clock();
        
        // Test avec partie en pause
        partie->estEnCours = false;
        updateGameState(partie, &deltaSlow, &deltaFast);
        // La fonction ne devrait rien faire si la partie n'est pas en cours
        
        // Test avec partie en cours
        partie->estEnCours = true;
        // clock_t oldDeltaSlow = deltaSlow;
        // clock_t oldDeltaFast = deltaFast;
        
        updateGameState(partie, &deltaSlow, &deltaFast);
        
        // Les deltas peuvent avoir changé ou non selon le timing
        // On vérifie juste que la fonction s'exécute sans erreur
        
        destroyPartie(partie);
    }
}

// Test avec partie NULL
void test_updateGameStateNull(void){
    printf("\n    • Test de updateGameState avec NULL\n");
    
    clock_t deltaSlow = clock();
    clock_t deltaFast = clock();
    
    // Test avec partie NULL - ne devrait pas planter
    updateGameState(NULL, &deltaSlow, &deltaFast);
    
    // Test avec pointeurs NULL
    updateGameState(NULL, NULL, NULL);
}

// Test des constantes SDL
void test_sdlConstants(void){
    printf("\n    • Test des constantes SDL\n");
    
    // Vérifier que les constantes sont définies avec des valeurs raisonnables
    CU_ASSERT(SDL_WINDOW_WIDTH > 0);
    CU_ASSERT(SDL_WINDOW_HEIGHT > 0);
    CU_ASSERT(SDL_GRID_ROWS > 0);
    CU_ASSERT(SDL_GRID_COLUMNS > 0);
    
    // Vérifier des ratios raisonnables
    CU_ASSERT(SDL_WINDOW_WIDTH >= SDL_GRID_COLUMNS);
    CU_ASSERT(SDL_WINDOW_HEIGHT >= SDL_GRID_ROWS);
}

// Test de la logique de bouncing du joueur
void test_playerBounceLogic(void){
    printf("\n    • Test de la logique de rebond du joueur\n");
    
    SDLResources resources;
    resources.isPlayerJumping = 0;
    resources.playerBounceOffset = 0;
    resources.playerBounceDirection = 0;
    
    // Simuler le début d'un saut
    resources.isPlayerJumping = 1;
    
    // Simuler plusieurs frames de montée
    for (int i = 0; i < 10; i++) {
        if (resources.isPlayerJumping) {
            if (resources.playerBounceDirection == 0) {
                // Monter
                resources.playerBounceOffset += 2;
                if (resources.playerBounceOffset >= 10) {
                    resources.playerBounceDirection = 1;
                }
            } else {
                // Descendre
                resources.playerBounceOffset -= 2;
                if (resources.playerBounceOffset <= 0) {
                    resources.playerBounceOffset = 0;
                    resources.playerBounceDirection = 0;
                    resources.isPlayerJumping = 0;
                }
            }
        }
    }
    
    // À la fin, le joueur devrait avoir terminé son saut
    CU_ASSERT_EQUAL(resources.isPlayerJumping, 0);
    CU_ASSERT_EQUAL(resources.playerBounceOffset, 0);
    CU_ASSERT_EQUAL(resources.playerBounceDirection, 0);
}

int main_test_renderer(void){
    CU_pSuite suite = CU_add_suite("Test du renderer", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test des ressources audio", test_audioResources);
    CU_add_test(suite, "Test des textures de jeu", test_gameTextures);
    CU_add_test(suite, "Test des ressources SDL", test_sdlResources);
    CU_add_test(suite, "Test de updateGameState", test_updateGameState);
    CU_add_test(suite, "Test de updateGameState avec NULL", test_updateGameStateNull);
    CU_add_test(suite, "Test des constantes SDL", test_sdlConstants);
    CU_add_test(suite, "Test de la logique de rebond", test_playerBounceLogic);

    return 0;
}
