#ifndef HELPER
#define HELPER


// -> Load / Save en JSON <- //
int loadIntVariable( char* variableName); 
void saveIntVariable(int variable, char* variableName);
char* loadCharVariable( char* variableName);
void saveCharVariable(char* variable, char* variableName);


// -> Terminal <- //
void setNonBlockingInput(void); 
void resetTerminal(void); 

#endif
