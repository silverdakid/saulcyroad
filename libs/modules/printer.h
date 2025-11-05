#ifndef PRINTER
#define PRINTER

// ------ IMPRESSION SUR LE TERMINAL DU MENU ------ //

void displayMenu(bool isValid);
int leaveMenu(void);

// ------ IMPRESSION POUR FAIRE UN CHOIX ------ //
bool makeChoice(char *question);

// ------ IMPRESSION SUR LE TERMINAL POUR LA PARTIE ------ //
void printStartPartie(bool isAnIa); 
void printPausePartie(int partieScore, int bestScore);  
void printEndPartie(int partieScore, int bestScore); 

#endif
