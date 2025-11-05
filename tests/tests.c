#include <CUnit/Basic.h>

#include "libs/types/test_types.h"
#include "libs/structures/test_joueur.h"
#include "libs/structures/test_statique.h"
#include "libs/structures/test_dynamique.h"
#include "libs/structures/test_ligne.h"
#include "libs/structures/test_partie.h"

#include "libs/structures/test_plateau.h"
#include "libs/structures/test_chemin.h"
#include "libs/structures/test_mapChemins.h"
#include "libs/modules/test_renderer.h"

int main(void) {
    CU_initialize_registry();

    // ---- Types ---- //
    int testResult = main_test_types();
    if(testResult != 0)  return testResult;
    
    // ---- Joueur ---- //
    testResult = main_test_joueur();
    if(testResult != 0)  return testResult;

    // ---- Statique ---- //
    testResult = main_test_statique();
    if(testResult != 0)  return testResult;

    // ---- Dynamique ---- //
    testResult = main_test_dynamique();
    if(testResult != 0)  return testResult;

    // ---- Ligne ---- //
    testResult = main_test_ligne();
    if(testResult != 0)  return testResult;

    // ---- Partie ---- //
    testResult = main_test_partie();
    if(testResult != 0)  return testResult;

    // ---- Helper ---- //
    // -> Nothing to TEST

    // ---- Printer ---- //
    // -> Nothing to TEST

    // ---- Plateau ---- //
    testResult = main_test_plateau();
    if(testResult != 0)  return testResult;

    // ---- Chemin ---- //
    testResult = main_test_chemin();
    if(testResult != 0)  return testResult;

    // ---- MapChemins ---- //
    testResult = main_test_mapChemins();
    if(testResult != 0)  return testResult;

    // ---- Renderer ---- //
    testResult = main_test_renderer();
    if(testResult != 0)  return testResult;

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();

    return 0;
}

