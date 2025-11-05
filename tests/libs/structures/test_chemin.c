#include <CUnit/Basic.h>

#include "../../../libs/types/types.h"
#include "../../../libs/structures/chemin.h"

// == createChemin et getter== //
void test_createChemin(void){
    printf("\n    • Création d'un chemin \n");
    Chemin* newChem = createChemin(12,8,5,BAS,NULL);
    
    printf("    • Accès à la coordonnée X\n");
    CU_ASSERT_EQUAL(getCheminCoordX(newChem),8);
    printf("    • Accès à la coordonnée Y\n");
    CU_ASSERT_EQUAL(getCheminCoordY(newChem),12);
    printf("    • Accès au socre\n");
    CU_ASSERT_EQUAL(getCheminScore(newChem),5);
    printf("    • Accès au type\n");
    CU_ASSERT_EQUAL(getCheminStartingDirection(newChem),BAS);
    printf("    • Accès au prochain\n");
    CU_ASSERT_EQUAL(getCheminNext(newChem),NULL);

    printf("    • Est une nouvelle meilleur direction -> cas HAUT \n");
    CU_ASSERT_TRUE(isANewBestDirection(BAS,HAUT,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(HAUT,HAUT,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(GAUCHE,HAUT,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(DROITE,HAUT,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(-1,HAUT,15,10));
    
    printf("    • Est une nouvelle meilleur direction -> cas BAS \n");
    CU_ASSERT_FALSE(isANewBestDirection(BAS,BAS,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(BAS,GAUCHE,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(BAS,DROITE,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(BAS,-1,15,10));

    printf("    • Est une nouvelle meilleur direction -> cas ATTENDRE \n");
    CU_ASSERT_FALSE(isANewBestDirection(-1,BAS,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(-1,-1,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(-1,GAUCHE,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(-1,DROITE,15,10));

    printf("    • Est une nouvelle meilleur direction -> cas GAUCHE \n");
    CU_ASSERT_FALSE(isANewBestDirection(DROITE,GAUCHE,5,10));
    CU_ASSERT_TRUE(isANewBestDirection(DROITE,GAUCHE,15,10));
    
    printf("    • Est une nouvelle meilleur direction -> cas DROITE \n");
    CU_ASSERT_FALSE(isANewBestDirection(GAUCHE,DROITE,15,10));
    CU_ASSERT_TRUE(isANewBestDirection(GAUCHE,DROITE,5,10));
    
    printf("    • Est une nouvelle meilleur direction -> Autres cas \n");
    CU_ASSERT_FALSE(isANewBestDirection(HAUT,DROITE,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(HAUT,GAUCHE,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(HAUT,BAS,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(HAUT,-1,15,10));

    CU_ASSERT_FALSE(isANewBestDirection(HAUT,DROITE,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(DROITE,DROITE,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(DROITE,-1,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(DROITE,BAS,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(GAUCHE,GAUCHE,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(GAUCHE,-1,15,10));
    CU_ASSERT_FALSE(isANewBestDirection(GAUCHE,BAS,15,10));

    printf("    • Set la meilleur direction -> oui \n");
    setBestDirection(newChem,HAUT,15,10);
    CU_ASSERT_EQUAL(getCheminStartingDirection(newChem),HAUT);

    printf("    • Set la meilleur direction -> non \n");
    setBestDirection(newChem,BAS,15,10);
    CU_ASSERT_EQUAL(getCheminStartingDirection(newChem),HAUT);

    printf("    • Destruction du chemin \n");
    destroyChemin(newChem);
}

// == createChemins== //
void test_createChemins(void){
    printf("\n    • Création d'un chemin \n");
    Chemin* newChems = createChemin(12,1,5,BAS,NULL);
    
    
    printf("    • Ajout de chemins\n");
    newChems= addChemin( createChemin(12,2,1,HAUT,NULL),newChems);
    newChems= addChemin( createChemin(12,3,10,GAUCHE,NULL),newChems);
    newChems= addChemin( createChemin(12,4,8,DROITE,NULL),newChems);
    newChems= addChemin( createChemin(12,5,20,BAS,NULL),newChems);
    
   
    CU_ASSERT_EQUAL(getCheminCoordX(newChems),5);
    CU_ASSERT_EQUAL(getCheminCoordX(getCheminNext(newChems)),3);
    CU_ASSERT_EQUAL(getCheminCoordX(getCheminNext(getCheminNext(newChems))),4);
    CU_ASSERT_EQUAL(getCheminCoordX(getCheminNext(getCheminNext(getCheminNext(newChems)))),1);
    CU_ASSERT_EQUAL(getCheminCoordX(getCheminNext(getCheminNext(getCheminNext(getCheminNext(newChems))))),2);



    CU_ASSERT_TRUE(isCheminAtCoord(12,5,newChems));
    printf("     •Deux chemins ont les mêmes coordonnées -> False \n");

    CU_ASSERT_FALSE(isCheminAtCoord(12,4,newChems));

    printf("    •Un chemin existe dans les existing -> True \n");
    CU_ASSERT_TRUE(isCheminEndExisting(12,4,0,HAUT,newChems,15,10));
    CU_ASSERT_EQUAL(getCheminStartingDirection(getCheminNext(getCheminNext(newChems))),DROITE);
    printf("    •Un chemin existe dans les existing -> False \n");
    CU_ASSERT_FALSE(isCheminEndExisting(12,10,0,BAS,newChems,15,10));

    printf("    • Obtenir la meilleur direction : cas 1\n");
    CU_ASSERT_EQUAL(getBestStartingDirection(newChems,10,15),BAS);
    printf("    • Obtenir la meilleur direction : cas 2\n");
    newChems->score=10;
    CU_ASSERT_EQUAL(getBestStartingDirection(newChems,10,15),GAUCHE);

    printf("    • Destruction des chemins\n");
    destroyAllChemin(newChems);
}


void test_cheminNull(void){

    printf("\n     • Accès à la coordonnée X de null\n");
    CU_ASSERT_EQUAL(getCheminCoordX(NULL),-1);
    printf("    • Accès à la coordonnée Y de null\n");
    CU_ASSERT_EQUAL(getCheminCoordX(NULL),-1);
    printf("    • Accès au socre de null\n");
    CU_ASSERT_EQUAL(getCheminScore(NULL),-1);
    printf("    • Accès au type de null\n");
    CU_ASSERT_EQUAL(getCheminStartingDirection(NULL),-1);
    printf("    • Accès au prochain de null\n");
    CU_ASSERT_EQUAL(getCheminNext(NULL),NULL);
    printf("    • Ajout de null\n");
    CU_ASSERT_EQUAL(addChemin(NULL,NULL),NULL);
    printf("    • À le même chemin que null\n");
    CU_ASSERT_FALSE(isCheminAtCoord(12,15,NULL));
    printf("    • la chemin null existe de  null\n");
    CU_ASSERT_FALSE(isCheminEndExisting(12,15,0,BAS,NULL,15,10));
    printf("    • Set la meilleur direction de null\n");
    setBestDirection(NULL,HAUT,15,10);
    printf("    • Obtenir la meilleur direction de null\n");
    CU_ASSERT_EQUAL(getBestStartingDirection(NULL,10,15),HAUT);

    printf("    • Destruction de rien\n");
    destroyChemin(NULL);
    destroyAllChemin(NULL);
}



int main_test_chemin(void){
    CU_pSuite suite = CU_add_suite("Test des chemins", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test la création et l'accès d'un chemin", test_createChemin);
    CU_add_test(suite, "Test la création et l'accès de plusieurs chemins", test_createChemins);
    CU_add_test(suite, "Test des cas Null", test_cheminNull);

    return 0;
}

