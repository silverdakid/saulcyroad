#include <CUnit/Basic.h>
#include <stdbool.h>

#include "../../../libs/structures/dynamique.h"


// == createStatique et getter== //
void test_createDynamiqueGet(void){
    printf("\n    • Création d'un dynamique \n");
    Dynamique* newDyn = createDynamique(8,3,0,BUCHE,NULL);
    
    printf("    • Accès à la coordonnée X\n");
    CU_ASSERT_EQUAL(getDynamiqueCoordX(newDyn),8);
    printf("    • Accès au tic\n");
    CU_ASSERT_EQUAL(getDynamiqueTicBeforeMove(newDyn),0);
    printf("    • Accès à la longueur\n");
    CU_ASSERT_EQUAL(getDynamiqueLongueur(newDyn),3);
    printf("    • Accès au type\n");
    CU_ASSERT_EQUAL(getDynamiqueType(newDyn),BUCHE);
    printf("    • Accès au prochain\n");
    CU_ASSERT_EQUAL(getDynamiqueNext(newDyn),NULL);

    printf("    • Déplacement du dynamique à gauche\n");
    moveDynamique(newDyn,GAUCHE,1);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(newDyn),7);
    printf("    • Déplacement du dynamique à droite\n");
    moveDynamique(newDyn,DROITE,1);
    moveDynamique(newDyn,DROITE,1);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(newDyn),9);

    printf("    • Est le dynamique à ces coordonnées : vrai\n");
    CU_ASSERT_TRUE(isDynamiqueAtCoord(newDyn,DROITE,9));
    printf("    • Est le dynamique à ces coordonnées avec la longueur: vrai \n");
    CU_ASSERT_TRUE(isDynamiqueAtCoord(newDyn,DROITE,7));
    printf("    • Est le dynamique à ces coordonnées après: faux\n");
    CU_ASSERT_FALSE(isDynamiqueAtCoord(newDyn,DROITE,10));
    printf("    • Est le dynamique à ces coordonnées avant: faux\n");
    CU_ASSERT_FALSE(isDynamiqueAtCoord(newDyn,DROITE,6));

    printf("    • Est le dynamique en dehors des limites: faux\n");
    CU_ASSERT_FALSE(isDynamiqueOutsideLigne(newDyn,GAUCHE,20));
    printf("    • Est le dynamique en dehors des limites à gauche: faux\n");
    newDyn->coordX = -1;
    CU_ASSERT_FALSE(isDynamiqueOutsideLigne(newDyn,GAUCHE,20));
    printf("    • Est le dynamique en dehors des limites à gauche: vrai\n");
    newDyn->coordX = -4;
    CU_ASSERT_TRUE(isDynamiqueOutsideLigne(newDyn,GAUCHE,20));
    printf("    • Est le dynamique en dehors des limites à droite: vrai\n");
    newDyn->coordX = 10;
    CU_ASSERT_TRUE(isDynamiqueOutsideLigne(newDyn,DROITE,8));

    printf("    • Formatage en String\n");
    CU_ASSERT_EQUAL(dynamiqueToString(newDyn,GAUCHE),typeDynamiqueToString(BUCHE,GAUCHE));

    printf("    • Copy du dynamique \n");
    Dynamique *copyDyn = copyDynamique(newDyn);
    CU_ASSERT_NOT_EQUAL(newDyn,copyDyn);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(copyDyn),10);
    CU_ASSERT_EQUAL(getDynamiqueLongueur(copyDyn),3);
    CU_ASSERT_EQUAL(getDynamiqueType(copyDyn),BUCHE);
    CU_ASSERT_EQUAL(getDynamiqueNext(copyDyn),NULL);

    printf("    • Destruction du dynamique \n");
    destroyDynamique(newDyn);
    destroyDynamique(copyDyn);
}

void test_createDynamiques(void){
    printf("\n    • Création d'un dynamique \n");
    Dynamique* newDyn = createDynamique(8,3,0,BUCHE,NULL);

    printf("    • Ajout de dynamique \n");
    newDyn = addDynamique(newDyn,createDynamique(1,2,0,BUCHE,NULL));
    newDyn = addDynamique(newDyn,createDynamique(14,4,0,BUCHE,NULL));

    printf("    • Vérification des ajouts\n");
    CU_ASSERT_EQUAL(getDynamiqueCoordX(newDyn),1);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(newDyn)),8);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(newDyn))),14);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(getDynamiqueNext(newDyn)))),-1);

    printf("    • Copy des dynamiques \n");
    Dynamique *copyDyn = copyDynamiques(newDyn);
    CU_ASSERT_NOT_EQUAL(newDyn,copyDyn);
    CU_ASSERT_NOT_EQUAL(getDynamiqueNext(newDyn),getDynamiqueNext(copyDyn));
    CU_ASSERT_NOT_EQUAL(getDynamiqueNext(getDynamiqueNext(newDyn)),getDynamiqueNext(getDynamiqueNext(copyDyn)));
    CU_ASSERT_EQUAL(getDynamiqueCoordX(copyDyn),1);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(copyDyn)),8);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(copyDyn))),14);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(getDynamiqueNext(copyDyn)))),-1);

    printf("    • Formatage en String des dynamiques\n");
    CU_ASSERT_EQUAL(dynamiqueToString(newDyn,GAUCHE),'<');
    CU_ASSERT_EQUAL(dynamiqueToString(getDynamiqueNext(newDyn),DROITE),'>');
    CU_ASSERT_EQUAL(dynamiqueToString(getDynamiqueNext(getDynamiqueNext(newDyn)),GAUCHE),'<');

    printf("    • Est le dynamique à ces coordonnées : vrai\n");
    CU_ASSERT_TRUE(isDynamiqueAtCoord(newDyn,GAUCHE,2));
    printf("    • Est le dynamique à ces coordonnées : faux\n");
    CU_ASSERT_FALSE(isDynamiqueAtCoord(newDyn,GAUCHE,15));

    printf("    • Est-il possible d'ajouter un dynamique à ces coords : vrai \n");
    CU_ASSERT_TRUE(isOneDynamiqueAtTheseCoord(newDyn,GAUCHE,15,3));
    printf("    • Est-il possible d'ajouter un dynamique à ces coords : false \n");
    CU_ASSERT_TRUE(isOneDynamiqueAtTheseCoord(newDyn,DROITE,15,3));


    printf("    • Déplacement des dynamiques à gauche\n");
    moveAllDynamique(newDyn,GAUCHE,20,1);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(newDyn),0);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(newDyn)),7);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(newDyn))),13);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(getDynamiqueNext(newDyn)))),-1);

    printf("    • Déplacement des dynamiques à droite\n");
    newDyn= moveAllDynamique(newDyn,DROITE,20,1);
    newDyn= moveAllDynamique(newDyn,DROITE,20,1);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(newDyn),2);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(newDyn)),9);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(newDyn))),15);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(getDynamiqueNext(newDyn)))),-1);

    printf("    • Déplacement des dynamiques en dehors de limites\n");
    newDyn= moveAllDynamique(newDyn,GAUCHE,15,1);
    newDyn= moveAllDynamique(newDyn,GAUCHE,15,1);
    newDyn= moveAllDynamique(newDyn,GAUCHE,15,1);
    newDyn= moveAllDynamique(newDyn,GAUCHE,15,1);
    newDyn= moveAllDynamique(newDyn,GAUCHE,15,1);
    newDyn= moveAllDynamique(newDyn,GAUCHE,15,1);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(newDyn),3);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(newDyn)),9);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getDynamiqueNext(getDynamiqueNext(newDyn))),-1);

    

    printf("    • Destruction des dynamiques \n");
    destroyAllDynamique(newDyn);
    destroyAllDynamique(copyDyn);
}

void test_createDynamiquesTrain(void){
    printf("\n    • Création d'un dynamique \n");
    Dynamique* newDyn = createDynamique(24,3,10,TRAIN,NULL);

    newDyn= moveAllDynamique(newDyn,GAUCHE,15,1);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(newDyn),24);
    CU_ASSERT_EQUAL(getDynamiqueTicBeforeMove(newDyn),9);

    printf("    • Destruction des dynamiques \n");
    destroyAllDynamique(newDyn);
}


void test_createNull(void){
    printf("    • Accès à la coordonnée X de null\n");
    CU_ASSERT_EQUAL(getDynamiqueCoordX(NULL),-1);
    printf("    • Accès à la longueur de null\n");
    CU_ASSERT_EQUAL(getDynamiqueLongueur(NULL),1);
    printf("    • Accès au tic de null\n");
    CU_ASSERT_EQUAL(getDynamiqueTicBeforeMove(NULL),0);
    printf("    • Accès au type de null\n");
    CU_ASSERT_EQUAL(getDynamiqueType(NULL),VOITURE);
    printf("    • Accès au prochain de Null\n");
    CU_ASSERT_EQUAL(getDynamiqueNext(NULL),NULL);

    printf("    • Ajout de Null à NULL\n");
    addDynamique(NULL,NULL);

    printf("    • Est en dehros de la ligne de null\n");
    CU_ASSERT_FALSE(isDynamiqueOutsideLigne(NULL,GAUCHE,20));
    printf("    • Est dynamique aux coordonnées de null\n");
    CU_ASSERT_FALSE(isDynamiqueAtCoord(NULL,GAUCHE,20));
    printf("    • Est ce qu'il y a au moins un dynamique aux coordonnées de null\n");
    CU_ASSERT_FALSE(isOneDynamiqueAtCoord(NULL,GAUCHE,20));
    printf("    • Est ce qu'il y a dynamique sur une plusieurs coordonnées de null\n");
    CU_ASSERT_FALSE(isOneDynamiqueAtTheseCoord(NULL,GAUCHE,20,20));

    printf("    • Déplacement de null\n");
    moveDynamique(NULL,GAUCHE,1);
    printf("    • Déplacement de tous les nulls\n");
    CU_ASSERT_EQUAL(moveAllDynamique(NULL,GAUCHE,20,1), NULL);

    printf("    • Formatage en String de null\n");
    CU_ASSERT_EQUAL(dynamiqueToString(NULL,GAUCHE),typeDynamiqueToString(VOITURE,GAUCHE));

    printf("    • Copy du null \n");
    CU_ASSERT_EQUAL(copyDynamique(NULL), NULL);
    CU_ASSERT_EQUAL(copyDynamiques(NULL), NULL);

    printf("    • Destruction du null \n");
    destroyDynamique(NULL);
    destroyAllDynamique(NULL);
}

void test_generationDynamique(void){
    printf("    •\n Generation d'une taille d'un dynamique\n");
    CU_ASSERT( 0 < generateSizeOfDynamique(TRAIN));
    printf("    • Generation d'une taille d'un dynamique null\n"); 
    CU_ASSERT( 0 < generateSizeOfDynamique(-1)); 

    printf("    • Generation d'un dynamique\n"); 
    Dynamique * newDyn = generateDynamique(RAPIDE,20,0, GAUCHE);
    CU_ASSERT_NOT_EQUAL(newDyn, NULL); 
    destroyDynamique(newDyn);
}



int main_test_dynamique(void){
    
    CU_pSuite suite = CU_add_suite("Test des dynamiques", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test la création et l'accès d'un dynamique", test_createDynamiqueGet);
    CU_add_test(suite, "Test la création et l'accès de plusieurs dynamiques", test_createDynamiques);
    CU_add_test(suite, "Test des cas TRAIN", test_createDynamiquesTrain);
    CU_add_test(suite, "Test des cas Null", test_createNull);
    CU_add_test(suite, "Test la génération des dynamiques", test_generationDynamique);

    return 0;
}

