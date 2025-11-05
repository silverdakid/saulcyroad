#include <CUnit/Basic.h>

#include "../../../libs/structures/mapChemins.h"

// == createStatique et getter== //
void test_mapChemins(void){
    printf("\n    • Création d'une map de chemins \n");
    MapChemins* newMap = createMapChemins(5);

    printf("    • Obtenir la taille de la map \n");
    CU_ASSERT_EQUAL(getMapCheminsSize(newMap),5);
    printf("    • Obtenir le tableau de chemins \n");
    Chemin ** tableau = getMapCheminsMap(newMap);
    CU_ASSERT_EQUAL(tableau[0],NULL);
    CU_ASSERT_EQUAL(tableau[1],NULL);
    CU_ASSERT_EQUAL(tableau[2],NULL);
    CU_ASSERT_EQUAL(tableau[3],NULL);
    CU_ASSERT_EQUAL(tableau[4],NULL);

    printf("    • Obtenir le chemin à l'index \n");
    CU_ASSERT_EQUAL(getMapCheminsAtIndex(newMap,0),NULL);

    printf("    • Obtenir la chemin de null à l'index négatif \n");
    CU_ASSERT_EQUAL(getMapCheminsAtIndex(newMap,-1),NULL);
    printf("    • Obtenir la chemin de null à l'index supérieur à la taille \n");
    CU_ASSERT_EQUAL(getMapCheminsAtIndex(newMap,6),NULL);

    printf("    • Ajouter des chemins dans les tableaux : cas 1 \n");
    addCheminInMapChemins(createChemin(12,1,5,BAS,NULL),newMap,2);
    CU_ASSERT_EQUAL(tableau[0],NULL);
    CU_ASSERT_EQUAL(tableau[1],NULL);
    CU_ASSERT_EQUAL(getCheminCoordX(tableau[2]),1);
    CU_ASSERT_EQUAL(tableau[3],NULL);
    CU_ASSERT_EQUAL(tableau[4],NULL);

    printf("    • Ajouter des chemins dans les tableaux : cas 2 \n");
    addCheminInMapChemins(createChemin(12,2,10,BAS,NULL),newMap,2);
    CU_ASSERT_EQUAL(tableau[0],NULL);
    CU_ASSERT_EQUAL(tableau[1],NULL);
    CU_ASSERT_EQUAL(getCheminCoordX(tableau[2]),2);
    CU_ASSERT_EQUAL(tableau[3],NULL);
    CU_ASSERT_EQUAL(tableau[4],NULL);

    printf("    • Ajouter des chemins dans les tableaux : cas 3 \n");
    addCheminInMapChemins(createChemin(12,3,5,BAS,NULL),newMap,4);
    CU_ASSERT_EQUAL(tableau[0],NULL);
    CU_ASSERT_EQUAL(tableau[1],NULL);
    CU_ASSERT_EQUAL(getCheminCoordX(tableau[2]),2);
    CU_ASSERT_EQUAL(tableau[3],NULL);
    CU_ASSERT_EQUAL(getCheminCoordX(tableau[4]),3);

    printf("    • Obtenir la chemin de null à l'index négatif \n");
    addCheminInMapChemins(createChemin(12,3,5,BAS,NULL),newMap,-1);
    CU_ASSERT_EQUAL(tableau[0],NULL);
    CU_ASSERT_EQUAL(tableau[1],NULL);
    CU_ASSERT_EQUAL(getCheminCoordX(tableau[2]),2);
    CU_ASSERT_EQUAL(tableau[3],NULL);
    CU_ASSERT_EQUAL(getCheminCoordX(tableau[4]),3);

    printf("    • Obtenir la chemin de null à l'index supérieur à la taille \n");
    addCheminInMapChemins(createChemin(12,3,5,BAS,NULL),newMap,6);
    CU_ASSERT_EQUAL(tableau[0],NULL);
    CU_ASSERT_EQUAL(tableau[1],NULL);
    CU_ASSERT_EQUAL(getCheminCoordX(tableau[2]),2);
    CU_ASSERT_EQUAL(tableau[3],NULL);
    CU_ASSERT_EQUAL(getCheminCoordX(tableau[4]),3);

   
    printf("    • Destruction de la map de chemins \n");
    destroyMapChemins(newMap);
}



void test_mapCheminsNull(void){
    printf("\n    • Création dune map de taille négative\n");
    CU_ASSERT_EQUAL(createMapChemins(-1),NULL);

    printf("    • Obtenir la taille de null \n");
    CU_ASSERT_EQUAL(getMapCheminsSize(NULL),-1);
    printf("    • Obtenir le tableau de null \n");
    CU_ASSERT_EQUAL(getMapCheminsMap(NULL),NULL);
    printf("    • Obtenir la chemin de null à l'index 0 \n");
    CU_ASSERT_EQUAL(getMapCheminsAtIndex(NULL,0),NULL);

    printf("    • Add la chemin de null à l'index 0 \n");
    addCheminInMapChemins(NULL,NULL,0);
   
    printf("    • Destruction de rien\n");
    destroyMapChemins(NULL);
}



int main_test_mapChemins(void){
    CU_pSuite suite = CU_add_suite("Test de la map de chemins", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test la création et l'accès d'une map de chemins", test_mapChemins);
    CU_add_test(suite, "Test des cas Null", test_mapCheminsNull);

    return 0;
}

