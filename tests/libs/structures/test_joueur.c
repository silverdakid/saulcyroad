#include <CUnit/Basic.h>
#include <stdbool.h>

#include "../../../libs/structures/joueur.h"
#include "../../../libs/types/types.h"


// == createJoueur et getter== //
void test_createAndGetter(void){
    printf("\n    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 2, 20, 3,CANARD,true,false);

    printf("    • Accès à la coordonnée Y\n");
    CU_ASSERT_EQUAL(getJoueurCoordY(newJoueur),8);
    printf("    • Accès à la coordonnée X\n");
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),2);
    printf("    • Accès à la limite de largueur X\n");
    CU_ASSERT_EQUAL(getJoueurLimites(newJoueur),20);
    printf("    • Regarde si le joueur est en vie X\n");
    CU_ASSERT_TRUE(isAlive(newJoueur));
    printf("    • Regarde si le joueur est une IA X\n");
    CU_ASSERT_FALSE(isAnIA(newJoueur));
    printf("    • accès au type du joueur X\n");
    CU_ASSERT_EQUAL(getJoueurType(newJoueur),CANARD);
    printf("    • accès au backward du joueur X\n");
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),3);

    printf("    • Destruction du joueur\n");
    destroyJoueur(newJoueur);
}

// == initialisation d'un joueur == //
void test_initialiseJoueur(void){
    printf("\n    • Création d'un joueur\n");
    Joueur* newJoueur = initialiseJoueur(POULET,true);

    printf("    • Accès à la coordonnée Y\n");
    CU_ASSERT_EQUAL(getJoueurCoordY(newJoueur),8);
    printf("    • Accès à la coordonnée X\n");
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),11);
    printf("    • Accès à la limite de largueur X\n");
    CU_ASSERT_EQUAL(getJoueurLimites(newJoueur),15);
    printf("    • Regarde si le joueur est en vie X\n");
    CU_ASSERT_TRUE(isAlive(newJoueur));
    printf("    • Regarde si le joueur est une IA X\n");
    CU_ASSERT_TRUE(isAnIA(newJoueur));
    printf("    • accès au type du joueur X\n");
    CU_ASSERT_EQUAL(getJoueurType(newJoueur),POULET);
    printf("    • accès au backward du joueur X\n");
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),0);

    printf("    • Destruction du joueur\n");
    destroyJoueur(newJoueur);
}

// == getter null== //
void test_getterNull(void){

    printf("    • Accès à la coordonnée Y\n");
    CU_ASSERT_EQUAL(getJoueurCoordY(NULL),-1);
    printf("    • Accès à la coordonnée X\n");
    CU_ASSERT_EQUAL(getJoueurCoordX(NULL),-1);
    printf("    • Accès à la limite de largueur X\n");
    CU_ASSERT_EQUAL(getJoueurLimites(NULL),-1);
    printf("    • Regarde si le joueur est en vie X\n");
    CU_ASSERT_FALSE(isAlive(NULL));
    printf("    • Regarde si le joueur est une IA X\n");
    CU_ASSERT_FALSE(isAnIA(NULL));
    printf("    • accès au type du joueur X\n");
    CU_ASSERT_EQUAL(getJoueurType(NULL),POULET);
    printf("    • accès au backward du joueur X\n");
    CU_ASSERT_EQUAL(getJoueurBackwards(NULL),-1);
}

// == addBackward  and Reduce == //
void test_addAndReduceBackward(void){
    printf("\n    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 2, 20, 0,CANARD,true,false);

    printf("    • Avancé du joueur\n");
    reduceBackward(newJoueur);
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),0);

    printf("    • Recule en arrière du joueur\n");
    addBackward(newJoueur);
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),1);
    addBackward(newJoueur);
    printf("    • Avancé du joueur\n");
    reduceBackward(newJoueur);
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),1);
    printf("    • Recule jusqu'à la mort du joueur\n");
    addBackward(newJoueur);
    addBackward(newJoueur);
    addBackward(newJoueur);
    addBackward(newJoueur);
    addBackward(newJoueur);
    addBackward(newJoueur);
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),7);
    CU_ASSERT_FALSE(isAlive(newJoueur));

    printf("    • Recule de null\n");
    addBackward(NULL);
    printf("    • Avancé de null\n");
    addBackward(NULL);

    printf("    • Destruction du joueur\n");
    destroyJoueur(newJoueur);
}


// == setter == //
void test_setter(void){
    printf("\n    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 2, 20, 2,CANARD,true,false);

    printf("    • Modification de la coordonnée X\n");
    setJoueurCoordX(newJoueur,16);
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),16);

    printf("    • Modification de la coordonnée Y\n");
    setJoueurCoordX(newJoueur,16);
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),16);

    printf("    • Tuer le joueur\n");
    killJoueur(newJoueur);
    CU_ASSERT_FALSE(isAlive(newJoueur));


    printf("    • Destruction du joueur\n");
    destroyJoueur(newJoueur);

    printf("    • Modifier les coord X de Null\n");
    setJoueurCoordX(NULL,16);
    printf("    • Modifier les coord Y de Null\n");
    setJoueurCoordY(NULL,16);
    printf("    • Tuer Null\n");
    killJoueur(NULL);
}



// == moveJoueur == //
void test_moveJoueur(void){
    printf("\n    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 15, 20, 1,CANARD,true,false);

    printf("    • Déplacement du joueur à gauche\n");
    moveJoueur(newJoueur,GAUCHE,false);
    CU_ASSERT_EQUAL(getJoueurCoordY(newJoueur),8);
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),14);

    printf("    • Déplacement du joueur à droite\n");
    moveJoueur(newJoueur,DROITE,false);
    CU_ASSERT_EQUAL(getJoueurCoordY(newJoueur),8);
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),15);


    printf("    • Déplacement du joueur en haut \n");
    moveJoueur(newJoueur,HAUT,false);
    CU_ASSERT_EQUAL(getJoueurCoordY(newJoueur),9);
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),15);
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),0);

    printf("    • Déplacement du joueur en bas \n");
    moveJoueur(newJoueur,BAS,false);
    CU_ASSERT_EQUAL(getJoueurCoordY(newJoueur),8);
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),15);
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),1);

    printf("    • Déplacement du joueur en bas jusqu'à la mort \n");
    moveJoueur(newJoueur,BAS,false);
    moveJoueur(newJoueur,BAS,false);
    moveJoueur(newJoueur,BAS,false);
    moveJoueur(newJoueur,BAS,false);
    moveJoueur(newJoueur,BAS,false);
    moveJoueur(newJoueur,BAS,false);
    CU_ASSERT_EQUAL(getJoueurCoordY(newJoueur),2);
    CU_ASSERT_EQUAL(getJoueurCoordX(newJoueur),15);
    CU_ASSERT_EQUAL(getJoueurBackwards(newJoueur),7);
    CU_ASSERT_FALSE(isAlive(newJoueur));

    printf("    • Déplacement de null\n");
    moveJoueur(NULL,BAS,false);
    moveJoueur(newJoueur,-1,false);

    printf("    • Destruction du joueur\n");
    destroyJoueur(newJoueur);
}

// == willBeInsideLimites == //
void test_willBeInsideLimites(void){
    printf("\n    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 2, 20, 1,CANARD,true,false);
    
    printf("\n    • Joueur sera dans la limite à la cord X \n");
    CU_ASSERT_TRUE(willBeInsideLimites(15, getJoueurLimites(newJoueur)));
    printf("\n    • Joueur ne sera pas dans la limite à droite la cord X \n");
    CU_ASSERT_FALSE(willBeInsideLimites(24, getJoueurLimites(newJoueur)));
    printf("\n    • Joueur ne sera pas dans la limite à gauche la cord X \n");
    CU_ASSERT_FALSE(willBeInsideLimites(3, getJoueurLimites(newJoueur)));


    printf("    • Destruction du joueur\n");
    destroyJoueur(newJoueur);
}

// == toString == //
void test_joueurToString(void){
    printf("\n    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 2, 20, 2,CANARD,true,false);

    printf("    • Obtention du joueur en string \n");
    CU_ASSERT_EQUAL(joueurToString(newJoueur),'*');

    printf("    • Obtention de null en string \n");
    CU_ASSERT_EQUAL(joueurToString(NULL),'?');

    printf("    • Destruction du joueur\n");
    destroyJoueur(newJoueur);
}


int main_test_joueur(void){

    CU_pSuite suite = CU_add_suite("Test du joueur", 0, 0);
    if (suite == NULL) {CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test de la création et l'accès au variable", test_createAndGetter);
    CU_add_test(suite, "Test de l'initialisation  et l'accès au variable ", test_initialiseJoueur);
    CU_add_test(suite, "Test de l'accès à null", test_getterNull);
    CU_add_test(suite, "Test des setters", test_setter);
    CU_add_test(suite, "Test de la gestion du backward", test_addAndReduceBackward);
    CU_add_test(suite, "Test du déplacement du joueur", test_moveJoueur);
    CU_add_test(suite, "Test de si le joueur est dans la limite", test_willBeInsideLimites);
    CU_add_test(suite, "Test de la convertion en string", test_joueurToString);


    
    return 0;
}

