#include <CUnit/Basic.h>
#include <stdlib.h> 

#include "../../../libs/structures/ligne.h"

// == createLignes== //
void test_createLigneGetAndSet(void){
    printf("\n    • Création d'un dynamique\n");
    Dynamique* newDyn = createDynamique(8,3,0,BUCHE,NULL);
    newDyn =addDynamique(newDyn, createDynamique(18,2,0,BUCHE,NULL));
    newDyn =addDynamique(newDyn,createDynamique(12,1,0,BUCHE,NULL));

    printf("    • Création d'un statique \n");
    Statique* newStat =createStatique(12,NENUPHARE,NULL);
    newStat = addStatique( newStat, createStatique(10,NENUPHARE,NULL));
    newStat = addStatique( newStat, createStatique(14,NENUPHARE,NULL));

    printf("    • Création d'une ligne statique\n");
    Ligne* newLigne = createLigne(8, FLEUVE, GAUCHE,newStat,newDyn,NULL  );

    printf("    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 2, 20, 3,CANARD,true,false);

    printf("    • Accès à la coordonées\n");
    CU_ASSERT_EQUAL(getLigneCoordY(newLigne),8);
    printf("    • Accès au type de la ligne \n");
    CU_ASSERT_EQUAL(getLigneType(newLigne),FLEUVE);
    printf("    • Accès à la direction de la ligne \n");
    CU_ASSERT_EQUAL(getLigneDirection(newLigne),GAUCHE);
    printf("    • Accès aux statiques ligne \n");
    CU_ASSERT_EQUAL(getLigneStatique(newLigne),newStat);
    printf("    • Accès aux dynamiques de la ligne \n");
    CU_ASSERT_EQUAL(getLigneDynamique(newLigne),newDyn);
    printf("    • Accès aux suivants de la ligne \n");
    CU_ASSERT_EQUAL(getLigneNext(newLigne),NULL);

    printf("    • EST-ce que la ligne est mortel : vrai \n");
    newLigne->type = FLEUVE;
    CU_ASSERT_TRUE(isLigneDeadly(newLigne));
    newLigne->type = RAPIDE;
    CU_ASSERT_TRUE(isLigneDeadly(newLigne));
    printf("    • EST-ce que la ligne est mortel : faux \n");
    newLigne->type = VOIE_FERREE;
    CU_ASSERT_FALSE(isLigneDeadly(newLigne));
    newLigne->type = ROUTE;
    CU_ASSERT_FALSE(isLigneDeadly(newLigne));
    newLigne->type = HERBE;
    CU_ASSERT_FALSE(isLigneDeadly(newLigne));

    printf("    • Est-ce que la ligne est statique : vrai \n");
    newLigne->type = HERBE;
    CU_ASSERT_TRUE(isLigneStatique(newLigne));
    newLigne->type = FLEUVE;
    CU_ASSERT_TRUE(isLigneStatique(newLigne));
    printf("    • EST-ce que la ligne est statique : faux \n");
    newLigne->type = VOIE_FERREE;
    CU_ASSERT_FALSE(isLigneStatique(newLigne));
    newLigne->type = ROUTE;
    CU_ASSERT_FALSE(isLigneStatique(newLigne));
    newLigne->type = RAPIDE;
    CU_ASSERT_FALSE(isLigneStatique(newLigne));

    printf("    • Est-ce que la ligne est à la coordY: vrai \n");
    CU_ASSERT_TRUE(isLigneAtCoordY(newLigne,8));
    printf("    • Est-ce que la ligne est à la coordY: false \n");
    CU_ASSERT_FALSE(isLigneAtCoordY(newLigne,2));

    printf("    • Est-ce que la ligne est lente: oui \n");
    newLigne->type = RAPIDE;
    CU_ASSERT_TRUE(isLigneSlow(newLigne));
    newLigne->type = ROUTE;
    CU_ASSERT_TRUE(isLigneSlow(newLigne));
    printf("    • Est-ce que la ligne est lente: faux \n");
    newLigne->type = VOIE_FERREE;
    CU_ASSERT_FALSE(isLigneSlow(newLigne));
    
    printf("    • Formatage en string de la ligne sans joueur et statique \n");
    newLigne->type = FLEUVE;
    newJoueur->coordY=9;
    char* ligne= ligneToString(newLigne,newJoueur);
    CU_ASSERT_STRING_EQUAL(ligne,"~~~~|~~~~~~o~o~o~~~~~~~~~|~~~~");
    free(ligne);
    printf("    • Formatage en string de la ligne avec joueur et statique \n");
    newJoueur->coordY=8;
    newJoueur->coordX=10;
    char* ligneTwo= ligneToString(newLigne,newJoueur);
    CU_ASSERT_STRING_EQUAL(ligneTwo,"~~~~|~~~~~~*~o~o~~~~~~~~~|~~~~");
    free(ligneTwo);
    printf("    • Formatage en string de la ligne sans joueur et dynamique \n");
    newJoueur->coordY=9;
    newLigne->type = RAPIDE;
    char* ligneTree= ligneToString(newLigne,newJoueur);
    CU_ASSERT_STRING_EQUAL(ligneTree,"~~~~|~~~~<<<~<~~~~~<<~~~~|~~~~");
    free(ligneTree);
    printf("    • Formatage en string de la ligne  avec joueur et dynamique \n");
    newJoueur->coordY=8;
    char* ligneFour= ligneToString(newLigne,newJoueur);
    CU_ASSERT_STRING_EQUAL(ligneFour,"~~~~|~~~~<<*~<~~~~~<<~~~~|~~~~");
    printf("    • Formatage en string de la ligne avec un train qui arrive \n");
    Dynamique* newDyna = createDynamique(32,3,10,TRAIN,NULL);
    destroyAllDynamique(newLigne->dynamiques);
    newLigne->dynamiques =newDyna;
    newJoueur->coordY=7;
    newLigne->type = VOIE_FERREE;

    char* ligneFifth= ligneToString(newLigne,newJoueur);
    CU_ASSERT_STRING_EQUAL(ligneFifth,"!==!|=!==!==!==!==!==!==!|=!==");
    free(ligneFifth);
    printf("    • Destruction de la ligne ( et des stats/dyn associés) \n");
    destroyJoueur(newJoueur);
    destroyLigne(newLigne);

}

// == createLignes== //
void test_createLignes(void){
    printf("\n    • Création de dynamique pour une ligne\n");
    Dynamique* newDyn = createDynamique(8,3,0,BUCHE,NULL);
    newDyn =addDynamique(newDyn, createDynamique(22,2,0,BUCHE,NULL));
    newDyn =addDynamique(newDyn, createDynamique(12,1,0,BUCHE,NULL));

    printf("    • Création de statique pour une ligne\n");
    Statique* newStat = createStatique(4,NENUPHARE,NULL);
    newStat = addStatique( newStat,createStatique(10,NENUPHARE,NULL));
    newStat = addStatique( newStat,createStatique(14,NENUPHARE,NULL));

    printf("\n    • Création de dynamique pour une autre ligne\n");
    Dynamique* newSecondDyn = createDynamique(3,3,0,VOITURE,NULL);
    newSecondDyn =addDynamique(newSecondDyn, createDynamique(17,2,0,CAMION,NULL));

    printf("    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 2, 20, 3,CANARD,true,false);

    printf("    • Création des lignes \n");
    Ligne* newLigne = createLigne(8, ROUTE, GAUCHE,NULL,newSecondDyn,NULL  );
    newLigne = createLigne(7, FLEUVE, GAUCHE,newStat,NULL,newLigne  );
    newLigne = createLigne(6, RAPIDE, DROITE,NULL,newDyn,newLigne  );

    printf("    • Obtention de la ligne à la coordonées Y 7 \n");
    CU_ASSERT_EQUAL(getLigneCoordY(getLigneAtCoordY(newLigne,7)),7);
    printf("    • Obtention de la ligne à la coordonées Y 12 \n");
    CU_ASSERT_EQUAL(getLigneAtCoordY(newLigne,12),NULL);

    printf("    • Obtention de la dernière ligne \n");
    CU_ASSERT_EQUAL(getLigneCoordY(getLastLigne(newLigne)),8);

    printf("    • Obtention de l'avant dernière ligne \n");
    CU_ASSERT_EQUAL(getLigneCoordY(getBeforeLastLigne(newLigne)),7);
    
    printf("    • EST-ce que la case est accessible oui : mortel \n");
    CU_ASSERT_TRUE(isCaseAccessible(newLigne,getJoueurCoordY(newJoueur)+6, getJoueurCoordX(newJoueur)+9));
    printf("    • EST-ce que la case est accessible oui : non mortel \n");
    CU_ASSERT_TRUE(isCaseAccessible(newLigne,getJoueurCoordY(newJoueur)+8, getJoueurCoordX(newJoueur)+5));
    printf("    • EST-ce que la case est accessible : non \n");
    newLigne->type=HERBE;
    CU_ASSERT_TRUE(isCaseAccessible(newLigne,getJoueurCoordY(newJoueur)+7, getJoueurCoordX(newJoueur)+10));
    newLigne->type=RAPIDE;

    printf("    • Est-ce qu'il y a une collision : non ( ligne non mortel ) \n");
    CU_ASSERT_FALSE(isColision(newLigne,getJoueurCoordY(newJoueur),getJoueurCoordX(newJoueur)));
    

    printf("    • Est-ce qu'il y a une collision : non ( ligne mortel ) \n");
    newJoueur->coordY=7;
    newJoueur->coordX=4;
    CU_ASSERT_FALSE(isColision(newLigne,getJoueurCoordY(newJoueur),getJoueurCoordX(newJoueur)));

    printf("    • Est-ce qu'il y a une collision : non ( ligne mortel ) \n");
    newJoueur->coordY=6;
    newJoueur->coordX=9;
    CU_ASSERT_TRUE(isColision(newLigne,getJoueurCoordY(newJoueur),getJoueurCoordX(newJoueur))); 

    printf("    • Est-ce qu'il y a une collision : non ( ligne mortel ) \n");
    newJoueur->coordY=6;
    newJoueur->coordX=8;
    CU_ASSERT_FALSE(isColision(newLigne,getJoueurCoordY(newJoueur),getJoueurCoordX(newJoueur)));

    printf("    • Est-ce qu'il y a une collision : non ( ligne non mortel ) \n");
    newJoueur->coordY=7;
    newJoueur->coordX=5;
    CU_ASSERT_TRUE(isColision(newLigne,getJoueurCoordY(newJoueur),getJoueurCoordX(newJoueur)));

    printf("    • Déplacement de tous les dynamiques lents sur la ligne \n");
    moveDynamiquesOnAllLignes(newLigne,newJoueur,true,1);
    CU_ASSERT_TRUE(isOneDynamiqueAtCoord(getLigneDynamique(getLigneAtCoordY(newLigne,6)),DROITE,9));
    CU_ASSERT_FALSE(isOneDynamiqueAtCoord(getLigneDynamique(getLigneAtCoordY(newLigne,6)),DROITE,10));
    CU_ASSERT_EQUAL(getStatiqueCoordX(getLigneStatique(getLigneAtCoordY(newLigne,7))),4);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getLigneDynamique(getLigneAtCoordY(newLigne,8))),2);

    printf("    • Déplacement de tous les dynamiques rapides sur la ligne  \n");
    moveDynamiquesOnAllLignes(newLigne,newJoueur,false,1);
    CU_ASSERT_TRUE(isOneDynamiqueAtCoord(getLigneDynamique(getLigneAtCoordY(newLigne,6)),DROITE,9));
    CU_ASSERT_FALSE(isOneDynamiqueAtCoord(getLigneDynamique(getLigneAtCoordY(newLigne,6)),DROITE,10));
    CU_ASSERT_EQUAL(getStatiqueCoordX(getLigneStatique(getLigneAtCoordY(newLigne,7))),4);
    CU_ASSERT_EQUAL(getDynamiqueCoordX(getLigneDynamique(getLigneAtCoordY(newLigne,8))),2);
    

    printf("    • Suppresion de la première ligne \n");
    newLigne = removeFirst(newLigne);
    CU_ASSERT_EQUAL(getLigneCoordY(newLigne),7);

    printf("    • Destruction de toutes les lignes \n");
    destroyJoueur(newJoueur);
    destroyAllLigne(newLigne);
}

// == createLignes== //
void test_generateLigne(void){
    printf("\n    • Création d'un joueur\n");
    Joueur* newJoueur = createJoueur(8, 2, 20, 3,CANARD,true,false);

    printf("    • Génération des lignes de départ\n");
    Ligne* newLigne = generateStartingLigne(newJoueur);
    CU_ASSERT_NOT_EQUAL(newLigne,NULL);

    printf("    • Génération d'une nouvelle ligne\n");
    Ligne* newLigneSecond = generateNewLigne(newLigne,newJoueur,0);
    CU_ASSERT_NOT_EQUAL(newLigneSecond,NULL);

    printf("    • Destruction de toutes les lignes et du joueur \n");
    destroyJoueur(newJoueur);
    destroyAllLigne(newLigne);
}

void test_nullLigne(void){
    printf("\n    • Accès à la coord Y de null\n");
    CU_ASSERT_EQUAL(getLigneCoordY(NULL),-1);
    printf("    • Accès au type de null\n");
    CU_ASSERT_EQUAL(getLigneType(NULL),HERBE);
    printf("    • Accès à la direction de null\n");
    CU_ASSERT_EQUAL(getLigneDirection(NULL),GAUCHE);
    printf("    • Accès aux statiques de null\n");
    CU_ASSERT_EQUAL(getLigneStatique(NULL),NULL);
    printf("    • Accès aux dynamiques de null\n");
    CU_ASSERT_EQUAL(getLigneDynamique(NULL),NULL);
    printf("    • Accès aux prochains de null\n");
    CU_ASSERT_EQUAL(getLigneNext(NULL),NULL);
    printf("    • Accès de la ligne à la coordonée y\n");
    CU_ASSERT_EQUAL(getLigneAtCoordY(NULL,3),NULL);

    printf("    • Null est mortel\n");
    CU_ASSERT_FALSE(isLigneDeadly(NULL));
    printf("    • Null est statique\n");
    CU_ASSERT_TRUE(isLigneStatique(NULL));
    printf("    • Est null à la coordonnées Y\n");
    CU_ASSERT_FALSE(isLigneAtCoordY(NULL,4));
    printf("    • La ligne à des véhicules slown\n");
    CU_ASSERT_TRUE(isLigneSlow(NULL));
    printf("    • Il y a collision entre null et null\n");
    CU_ASSERT_FALSE(isColision(NULL,0,0));
    printf("    • La case est accessible de NULL\n");
    CU_ASSERT_FALSE(isCaseAccessible(NULL,0,1));

    printf("    • Supprime le premier de null\n");
    CU_ASSERT_EQUAL(removeFirst(NULL),NULL);
    printf("    • Obtenir le dernier de null\n");
    CU_ASSERT_EQUAL(getLastLigne(NULL),NULL);

    printf("    • Obtentir de l'avant dernière ligne de NULL\n");
    CU_ASSERT_EQUAL(getBeforeLastLigne(NULL),NULL);
    printf("    • Obtenir la dernière ligne statique à partir de NULL\n");
    CU_ASSERT_EQUAL(getLastStatiqueLigne(NULL),NULL);

    printf("    • Déplacement de tous les dynamiques sur null\n");
    moveDynamiquesOnAllLignes(NULL,NULL,false,1);

    printf("    • Génération des lignes de départ de null \n");
    CU_ASSERT_EQUAL(generateStartingLigne(NULL),NULL);
    printf("    • Génération des nouvelles lignes de null \n");
    CU_ASSERT_EQUAL(generateNewLigne(NULL,NULL,0),NULL);

    printf("    • To String de Null \n");
    CU_ASSERT_STRING_EQUAL(ligneToString(NULL,NULL),"\n");

    printf("    • Destruction de Null \n");
    destroyLigne(NULL);
    destroyAllLigne(NULL);
}

int main_test_ligne(void){

    CU_pSuite suite = CU_add_suite("Test des lignes", 0, 0);
    if (suite == NULL){ CU_cleanup_registry();  return CU_get_error();}
    
    CU_add_test(suite, "Test de la  création, setter et getter d'une ligne", test_createLigneGetAndSet);
    CU_add_test(suite, "Test de lignes", test_createLignes);
    CU_add_test(suite, "Test de la génération", test_generateLigne);
    CU_add_test(suite, "Test des cas null", test_nullLigne);

    return 0;
}
