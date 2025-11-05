#include <stdio.h>
#include <stdbool.h>

#include "libs/modules/printer.h"
#include "libs/structures/partie.h"


int main(void){ 
    char choix;
    bool isInvalid = true;

    while (1) { 
        displayMenu(isInvalid);
        scanf("%c", &choix);

        switch (choix) {
            case '1':
                isInvalid=true;
                printf("\nLancement en mode terminal...\n\n");
                playPartie();
                break;
            case '2':
                isInvalid=true;
                printf("Lancement en mode graphique...\n");
                showGraphicalMenu();
                break;
            case '3':
                leaveMenu();
                return 0;
            default:
                isInvalid=false;
        }
    }
    return 0;
}
