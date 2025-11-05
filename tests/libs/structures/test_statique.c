#include <CUnit/Basic.h>

#include "../../../libs/structures/statique.h"

// == createStatique et getter== //
void test_createStatiqueGet(void){
    printf("\n    • Création d'un statique \n");
    Statique* newStat = createStatique(12,NENUPHARE,NULL);
    

    printf("    • Accès à la coordonnée X\n");
    CU_ASSERT_EQUAL(getStatiqueCoordX(newStat),12);
    printf("    • Accès au type\n");
    CU_ASSERT_EQUAL(getStatiqueType(newStat),NENUPHARE);
    printf("    • Accès au prochain\n");
    CU_ASSERT_EQUAL(getStatiqueNext(newStat),NULL);

    printf("    • Est le statique à la coordonnée voulu ou non\n");
    CU_ASSERT_TRUE(isStatiqueAtCoord(newStat,12));
    CU_ASSERT_FALSE(isStatiqueAtCoord(newStat,10));

    printf("    • Formatage en String\n");
    CU_ASSERT_EQUAL(statiqueToString(newStat),typeStatiqueToString(NENUPHARE));

    printf("    • Destruction du statique \n");
    destroyStatique(newStat);
}

// == createStatique et getter== //
void test_createStatiques(void){
    printf("\n    • Création d'un statique \n");
    Statique* newStat = createStatique(12,NENUPHARE,NULL);
    
    printf("    • Ajout de statiques\n");
    newStat= addStatique(newStat,createStatique(10,NENUPHARE,NULL));
    newStat= addStatique(newStat,createStatique(8,NENUPHARE,NULL));
    CU_ASSERT_EQUAL(getStatiqueCoordX(newStat),8);
    CU_ASSERT_EQUAL(getStatiqueCoordX(getStatiqueNext(newStat)),10);
    CU_ASSERT_EQUAL(getStatiqueCoordX(getStatiqueNext(getStatiqueNext(newStat))),12);

    printf("    • Est-ce qu'il existe un statique à une coordonnée\n");
    CU_ASSERT_TRUE(isStatiqueExists(newStat,12,isStatiqueAtCoord));
    CU_ASSERT_FALSE(isStatiqueExists(newStat,9,isStatiqueAtCoord));

    printf("    • Suppression d'un statique\n");
    removeStatiqueAtCoord(newStat,10);
    removeStatiqueAtCoord(newStat,10);
    removeStatiqueAtCoord(newStat,-1);
    CU_ASSERT_EQUAL(getStatiqueCoordX(newStat),8);
    CU_ASSERT_EQUAL(getStatiqueCoordX(getStatiqueNext(newStat)),12);

    printf("    • Destruction des statiques\n");
    destroyStatique(newStat);
}


void test_statiqueNull(void){
    printf("    • Accès à la coordonnée X\n");
    CU_ASSERT_EQUAL(getStatiqueCoordX(NULL),-1);
    printf("    • Accès à la coordonnée T\n");
    CU_ASSERT_EQUAL(getStatiqueType(NULL),ARBRE);
    printf("    • Accès au prochain de null\n");
    CU_ASSERT_EQUAL(getStatiqueNext(NULL),NULL);
    printf("    • Ajout de NULL\n");
    CU_ASSERT_EQUAL(addStatique(NULL,NULL),NULL);
    printf("    • Est null au coordonnées \n");
    CU_ASSERT_FALSE(isStatiqueAtCoord(NULL,-1));
    printf("    • Formatage en string de NULL \n");
    CU_ASSERT_EQUAL(statiqueToString(NULL),'?');
    printf("    • Suppression de NULL \n");
    removeStatiqueAtCoord(NULL,0);

    printf("    • Destruction de rien\n");
    destroyStatique(NULL);
}

void test_generateStatiques(void){
    printf("\n    • Génération d'un statique \n");
    Statique* newStat = generateStatique(FLEUVE,20,0);

    printf("\n    • Accès à ses paramètres \n");
    CU_ASSERT_NOT_EQUAL(newStat,NULL);
    CU_ASSERT_NOT_EQUAL(getStatiqueCoordX(newStat),-1);

    printf("    • Destruction des statiques\n");
    destroyStatique(newStat);
}

int main_test_statique(void){
    CU_pSuite suite = CU_add_suite("Test des statiques", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test la création et l'accès d'un statique", test_createStatiqueGet);
    CU_add_test(suite, "Test la création et l'accès de plusieurs statiques", test_createStatiques);
    CU_add_test(suite, "Test des cas Null", test_statiqueNull);
    CU_add_test(suite, "Test la génération des statiques", test_generateStatiques);

    return 0;
}

