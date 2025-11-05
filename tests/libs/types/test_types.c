
#include <CUnit/Basic.h>

#include "../../../libs/types/types.h"



// == typeStatiqueToString == //
void test_typeStatiqueToString(void){
    printf("\n    • Convertion de Nenuphare en String\n");
    CU_ASSERT_EQUAL(typeStatiqueToString(NENUPHARE),'o');
    printf("    • Convertion de Buisson en String\n");
    CU_ASSERT_EQUAL(typeStatiqueToString(BUISSON),'@');
    printf("    • Convertion d'Arbre en String\n");
    CU_ASSERT_EQUAL(typeStatiqueToString(ARBRE),'#');
    printf("    • Convertion de -1 en String\n");
    CU_ASSERT_EQUAL(typeStatiqueToString(-1),'?');
}

// == typeDynamiqueToString == //
void test_typeDynamiqueToString(void){

    // --- Voiture --- //
    printf("\n    • Convertion d'une voiture allant à gauche en String\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(VOITURE, GAUCHE),'<');
    printf("    • Convertion d'une voiture allant à droite en String\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(VOITURE, DROITE),'>');

    // --- Camion --- //
    printf("    • Convertion d'un camion allant à gauche en String\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(CAMION, GAUCHE),'<');
    printf("    • Convertion d'un camion allant à droite en String\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(CAMION, DROITE),'>');

    // --- Train --- //
    printf("    • Convertion d'un train allant à gauche en String\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(TRAIN, GAUCHE),'<');
    printf("    • Convertion d'un train allant à droite en String\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(TRAIN, DROITE),'>');

    // --- Buche --- //
    printf("    • Convertion d'une buche allant à gauche en String\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(BUCHE, GAUCHE),'<');
    printf("    • Convertion d'une buche allant à droite en String\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(BUCHE, DROITE),'>');
    
    // --- Autres --- //
    printf("    • Convertion d'autre chose\n");
    CU_ASSERT_EQUAL(typeDynamiqueToString(-1, -1),'?');
}

// == typeLigneToString == //
void test_typeLigneToString(void){
    printf("\n    • Convertion de la route en String\n");
    CU_ASSERT_EQUAL(typeLigneToString(ROUTE),'-');
    printf("    • Convertion de la voie ferrée en String\n");
    CU_ASSERT_EQUAL(typeLigneToString(HERBE),'\"');
    printf("    • Convertion de la route en String\n");
    CU_ASSERT_EQUAL(typeLigneToString(VOIE_FERREE),'=');
    printf("    • Convertion du fleuve en String\n");
    CU_ASSERT_EQUAL(typeLigneToString(FLEUVE),'~');
    printf("    • Convertion des rapides en String\n");
    CU_ASSERT_EQUAL(typeLigneToString(RAPIDE),'~');
    printf("    • Convertion d'autre chose en String\n");
    CU_ASSERT_EQUAL(typeLigneToString(-1),'?');
}

// == typeJoueurToString == //
void test_typeJoueurToString(void){
    printf("\n    • Convertion du poulet en String\n");
    CU_ASSERT_EQUAL(typeJoueurToString(POULET),'*');
    printf("    • Convertion du canard en String\n");
    CU_ASSERT_EQUAL(typeJoueurToString(CANARD),'*');
    printf("    • Convertion d'autre chose en String\n");
    CU_ASSERT_EQUAL(typeJoueurToString(-1),'?');
}

// == getDirectionNextCoord == //
void test_getDirectionNextCoord(void){
    printf("\n    • Conversion en Coordonées X\n");
    CU_ASSERT_EQUAL(getDirectionNextCoordX(GAUCHE),-1);
    CU_ASSERT_EQUAL(getDirectionNextCoordX(DROITE),1);
    CU_ASSERT_EQUAL(getDirectionNextCoordX(HAUT),0);
    CU_ASSERT_EQUAL(getDirectionNextCoordX(BAS),0);
    CU_ASSERT_EQUAL(getDirectionNextCoordX(-1),0);
    CU_ASSERT_EQUAL(getDirectionNextCoordX(NB_DIRECTIONS),0);

    printf("\n    • Conversion en Coordonées Y\n");
    CU_ASSERT_EQUAL(getDirectionNextCoordY(GAUCHE),0);
    CU_ASSERT_EQUAL(getDirectionNextCoordY(DROITE),0);
    CU_ASSERT_EQUAL(getDirectionNextCoordY(HAUT),1);
    CU_ASSERT_EQUAL(getDirectionNextCoordY(BAS),-1);
    CU_ASSERT_EQUAL(getDirectionNextCoordY(-1),0);
    CU_ASSERT_EQUAL(getDirectionNextCoordY(NB_DIRECTIONS),0);
}


int main_test_types(void){

    CU_pSuite suite = CU_add_suite("Test des types", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test la convertion des types statiques en string", test_typeStatiqueToString);
    CU_add_test(suite, "Test la convertion des types dynamiques en string", test_typeDynamiqueToString);
    CU_add_test(suite, "Test la convertion des types lignes en string", test_typeLigneToString);
    CU_add_test(suite, "Test la convertion des types joueurs en string", test_typeJoueurToString);
    CU_add_test(suite, "Test la convertion du types direction en coordonnées", test_getDirectionNextCoord);

    return 0;
}

