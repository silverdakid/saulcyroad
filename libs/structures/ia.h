#ifndef IA
#define IA

#define PROFONDEUR 25

#include <time.h>
#include "ia.h"
#include "ligne.h"
#include "joueur.h"
#include "chemin.h"
#include "../types/types.h"


// =============== Fonctions =============== // 

int notePosition(int startingX, int startingY, int endingX, int endingY, int centerX);
Direction bestDirectionToGo(Ligne* lignes, Joueur* currentJoueur,clock_t currentClock);


#endif
