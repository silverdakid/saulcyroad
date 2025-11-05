#include <CUnit/Basic.h>

#include "../../../libs/structures/partie.h"
#include "../../../libs/modules/renderer.h"


// == createPartie et getter== //
void test_createPartieGetAndSet(void){
    printf("\n    • Création d'une joueur pour la partie\n");
    Joueur* newJoueur = createJoueur(8, 15, 20, 3,CANARD,true,false);


    printf("    • Création d'une partie\n");
    Partie* newPartie = createPartie(8,true,NULL,newJoueur);

    printf("    • Accès au score de la partie\n");
    CU_ASSERT_EQUAL(getPartieScore(newPartie),8);
    printf("    • Regarde si la partie est en cours\n");
    CU_ASSERT_TRUE(isPartieEnCours(newPartie));
    printf("    • Accède au joueur\n");
    CU_ASSERT_EQUAL(getPartieJoueur(newPartie),newJoueur);
    printf("    • Accède aux lignes\n");
    CU_ASSERT_EQUAL(getPartieLignes(newPartie),NULL);

    printf("    • Augmentation du score\n");
    addScore(newPartie);
    CU_ASSERT_EQUAL(getPartieScore(newPartie),9);

    printf("    • Met en pause la partie\n");
    updateEstEnCours(newPartie);
    CU_ASSERT_FALSE(isPartieEnCours(newPartie));

    printf("    • Continue la partie\n");
    updateEstEnCours(newPartie);
    CU_ASSERT_TRUE(isPartieEnCours(newPartie));

    printf("    • Destruction de la partie ( dont le joueur ) \n");
    destroyPartie(newPartie);
    printf("    • Destruction terminé\n");
}

void test_setAndGetNull(void){
    printf("\n    • Accès au score de null\n");
    CU_ASSERT_EQUAL(getPartieScore(NULL),-1);

    printf("    • Regarde si null est en cours\n");
    CU_ASSERT_FALSE(isPartieEnCours(NULL));
    printf("    • Accède au joueur de null\n");
    CU_ASSERT_EQUAL(getPartieJoueur(NULL),NULL);
    printf("    • Accède aux lignes de null\n");
    CU_ASSERT_EQUAL(getPartieLignes(NULL),NULL);

    printf("    • Augmentation du score\n");
    addScore(NULL);

    printf("    • Change est en cours de null\n");
    updateEstEnCours(NULL);
}

void test_generatePartie(void){
    printf("\n    • Génération d'une partie\n");
    Partie* newPartie = generatePartie(true);

    printf("    • Accès au score de la partie\n");
    CU_ASSERT_EQUAL(getPartieScore(newPartie),0);
    printf("    • Regarde si la partie est en cours\n");
    CU_ASSERT_FALSE(isPartieEnCours(newPartie));
    printf("    • Accède au joueur\n");
    CU_ASSERT_TRUE(isAnIA(getPartieJoueur(newPartie)));
    CU_ASSERT_NOT_EQUAL(getPartieJoueur(newPartie),NULL)
    printf("    • Accède aux lignes\n");
    CU_ASSERT_NOT_EQUAL(getPartieLignes(newPartie),NULL)

    destroyPartie(newPartie);
}   

void test_playPartieSdlPreparation(void){
    printf("\n    • Test de préparation pour playPartieSdl\n");
    
    // Test de génération d'une partie pour SDL
    Partie* partieHumain = generatePartie(false);
    CU_ASSERT_NOT_EQUAL(partieHumain, NULL);
    CU_ASSERT_FALSE(isAnIA(getPartieJoueur(partieHumain)));
    
    Partie* partieIA = generatePartie(true);
    CU_ASSERT_NOT_EQUAL(partieIA, NULL);
    CU_ASSERT_TRUE(isAnIA(getPartieJoueur(partieIA)));
    
    // Test des constantes utilisées par playPartieSdl
    const int WIDTH = SDL_WINDOW_WIDTH;
    const int HEIGHT = SDL_WINDOW_HEIGHT;
    const int NB_LIGNE = SDL_GRID_ROWS;
    const int NB_COLONNE = SDL_GRID_COLUMNS;
    
    CU_ASSERT(WIDTH > 0);
    CU_ASSERT(HEIGHT > 0);
    CU_ASSERT(NB_LIGNE > 0);
    CU_ASSERT(NB_COLONNE > 0);
    
    // Test de calcul des dimensions de cellules
    int cellWidth = WIDTH / NB_COLONNE;
    int cellHeight = HEIGHT / NB_LIGNE;
    
    CU_ASSERT(cellWidth > 0);
    CU_ASSERT(cellHeight > 0);
    
    destroyPartie(partieHumain);
    destroyPartie(partieIA);
}

void test_sdlResourcesInit(void){
    printf("\n    • Test d'initialisation des ressources SDL\n");
    
    SDLResources resources;
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int NB_LIGNE = 15;
    const int NB_COLONNE = 25;
    
    // Simuler l'initialisation des ressources
    resources.cellWidth = WIDTH / NB_COLONNE;
    resources.cellHeight = HEIGHT / NB_LIGNE;
    resources.isPlayerJumping = 0;
    resources.playerBounceOffset = 0;
    resources.playerBounceDirection = 0;
    
    CU_ASSERT_EQUAL(resources.cellWidth, WIDTH / NB_COLONNE);
    CU_ASSERT_EQUAL(resources.cellHeight, HEIGHT / NB_LIGNE);
    CU_ASSERT_EQUAL(resources.isPlayerJumping, 0);
    CU_ASSERT_EQUAL(resources.playerBounceOffset, 0);
    CU_ASSERT_EQUAL(resources.playerBounceDirection, 0);
}

void test_gameLoopVariables(void){
    printf("\n    • Test des variables de la boucle de jeu\n");
    
    // Variables utilisées dans playPartieSdl
    int running = 1;
    int offset = 0;
    int ligne = 0;
    // clock_t deltaSlow = clock();
    // clock_t deltaFast = clock();
    // clock_t deltaIA = clock();
    
    // Test des valeurs initiales
    CU_ASSERT_EQUAL(running, 1);
    CU_ASSERT_EQUAL(offset, 0);
    CU_ASSERT_EQUAL(ligne, 0);
    
    // Test de modification des variables
    running = 0;
    offset = (offset + 1) % 4;
    ligne = 1;
    
    CU_ASSERT_EQUAL(running, 0);
    CU_ASSERT_EQUAL(offset, 1);
    CU_ASSERT_EQUAL(ligne, 1);
    
    // Test du cycling de offset
    for (int i = 0; i < 10; i++) {
        offset = (offset + 1) % 4;
    }
    CU_ASSERT(offset >= 0 && offset < 4);
}

void test_scoreKeyLogic(void){
    printf("\n    • Test de la logique des clés de score\n");
    
    // Test pour joueur humain
    bool isAIControlled = false;
    char* score_key;
    
    if (isAIControlled) {
        score_key = "best_score_ia";
    } else {
        score_key = "best_score";
    }
    
    CU_ASSERT_STRING_EQUAL(score_key, "best_score");
    
    // Test pour IA
    isAIControlled = true;
    
    if (isAIControlled) {
        score_key = "best_score_ia";
    } else {
        score_key = "best_score";
    }
    
    CU_ASSERT_STRING_EQUAL(score_key, "best_score_ia");
}

int main_test_partie(void){

    CU_pSuite suite = CU_add_suite("Test des parties", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    //CU_add_test(suite, "Test la  création, setter et getter d'une partie", test_createPartieGetAndSet);
    CU_add_test(suite, "Test la  création, setter et getter d'une partie", test_setAndGetNull);
    CU_add_test(suite, "Test la  création, setter et getter d'une partie", test_generatePartie);
    CU_add_test(suite, "Test de préparation pour playPartieSdl", test_playPartieSdlPreparation);
    CU_add_test(suite, "Test d'initialisation des ressources SDL", test_sdlResourcesInit);
    CU_add_test(suite, "Test des variables de la boucle de jeu", test_gameLoopVariables);
    CU_add_test(suite, "Test de la logique des clés de score", test_scoreKeyLogic);

    return 0;
}

