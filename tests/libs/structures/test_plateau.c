#include <CUnit/Basic.h>

#include "../../../libs/types/types.h"
#include "../../../libs/structures/plateau.h"

// == createplateau et getter== //
void test_createplateau(void){
    printf("\n    • Création d'un plateau \n");
    Plateau* newPlat = createPlateau(createLigne(5,RAPIDE,HAUT,NULL,NULL,NULL),false, NULL);
    
    printf("    • Accès aux lignes\n");
    CU_ASSERT_EQUAL(getLigneCoordY(getPlateauLigne(newPlat)),5);
    printf("    • Si les dynamiques ont bougé juste avant\n");
    CU_ASSERT_EQUAL(haveDynamiquesMoves(newPlat),false);
    printf("    • Obtenir le prochain plateau\n");
    CU_ASSERT_EQUAL(getPlateauNext(newPlat),NULL);

    printf("    • Destruction du plateau \n");
    destroyPlateau(newPlat);
}

// == createplateaus== //
void test_createPlateaux(void){
    printf("\n    • Création d'un plateau \n");
    Plateau* newPlateaux = createPlateau(createLigne(5,RAPIDE,HAUT,NULL,NULL,NULL),false, NULL);

    printf("    • Ajout de plateaux\n");
    newPlateaux= addPlateau(newPlateaux,createPlateau(createLigne(8,FLEUVE,HAUT,NULL,NULL,NULL),false, NULL));
    newPlateaux= addPlateau(newPlateaux, createPlateau(createLigne(6,RAPIDE,HAUT,NULL,NULL,NULL),false, NULL));
    
    CU_ASSERT_EQUAL(getLigneCoordY(getPlateauLigne(newPlateaux)),5);
    CU_ASSERT_EQUAL(getLigneCoordY(getPlateauLigne(getPlateauNext(newPlateaux))),8);
    CU_ASSERT_EQUAL(getLigneCoordY(getPlateauLigne(getPlateauNext(getPlateauNext(newPlateaux)))),6);

    printf("     •Suppression du premier plateau \n");
    newPlateaux = removeFirstPlateau(newPlateaux);

    CU_ASSERT_EQUAL(getLigneCoordY(getPlateauLigne(newPlateaux)),8);
    CU_ASSERT_EQUAL(getLigneCoordY(getPlateauLigne(getPlateauNext(newPlateaux))),6);

    printf("    • Destruction des plateaus\n");
    destroyAllPlateau(newPlateaux);
}


void test_plateauNull(void){

    printf("\n     • Accès aux lignes de null\n");
    CU_ASSERT_EQUAL(getPlateauLigne(NULL),NULL);
    printf("    • À si les dynamqiues ont bougé de null\n");
    CU_ASSERT_TRUE(haveDynamiquesMoves(NULL));
    printf("    • Accès au prochain plateau de null\n");
    CU_ASSERT_EQUAL(getPlateauNext(NULL),NULL);
    printf("    • Ajouter null à null\n");
    CU_ASSERT_EQUAL(addPlateau(NULL,NULL),NULL);
    printf("    • Suppression de null \n");
    CU_ASSERT_EQUAL(removeFirstPlateau(NULL),NULL);
    printf("    • Génération de null\n");
    CU_ASSERT_EQUAL(generatePlateaux(NULL,20,20,NULL),NULL);

    printf("    • Destruction de null\n");
    destroyPlateau(NULL);
    destroyAllPlateau(NULL);
}



int main_test_plateau(void){
    CU_pSuite suite = CU_add_suite("Test des plateaux", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test la création et l'accès d'un plateau", test_createplateau);
    CU_add_test(suite, "Test la création et l'accès de plusieurs plateaux", test_createPlateaux);
    CU_add_test(suite, "Test des cas Null", test_plateauNull);

    return 0;
}

