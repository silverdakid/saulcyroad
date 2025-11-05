#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#include "helper.h"
#include "modules/cJSON.h"

struct termios oldt, newt;

int loadIntVariable( char* variableName){
    FILE *file = fopen("assets/data.json", "r");
    if (!file) return -1; 

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0'; 

    cJSON *json = cJSON_Parse(data);
    free(data);

    if (!json) return 0;

    cJSON *jsonVariable = cJSON_GetObjectItem(json, variableName);
    int variable = jsonVariable ? jsonVariable->valueint : 0;

    cJSON_Delete(json);
    return variable;
}

void saveIntVariable(int variable, char* variableName){ 
    cJSON *json = NULL;
    FILE *file = fopen("assets/data.json", "r");
    long length = 0;
    char *data = NULL;

    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        data = (char *)malloc(length + 1);
        if (data != NULL) {
            fread(data, 1, length, file);
            data[length] = '\0';
            json = cJSON_Parse(data);
        }
        fclose(file);
        free(data);
    }

    if (json == NULL) { // Si le fichier n'existe pas, est vide ou contient un JSON invalide
        json = cJSON_CreateObject();
    }

    if (cJSON_HasObjectItem(json, variableName)) {
        cJSON_ReplaceItemInObject(json, variableName, cJSON_CreateNumber(variable));
    } else {
        cJSON_AddNumberToObject(json, variableName, variable);
    }

    char *json_data = cJSON_Print(json);
    file = fopen("assets/data.json", "w");

    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        free(json_data);
        cJSON_Delete(json);
        return;
    }

    fprintf(file, "%s", json_data);
    fclose(file);

    free(json_data);
    cJSON_Delete(json);
}

void setNonBlockingInput(void) {
    
    tcgetattr(STDIN_FILENO, &oldt);  
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

   
}

void resetTerminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);

    newt = oldt;  
}

char* loadCharVariable( char* variableName){
    FILE *file = fopen("assets/data.json", "r");
    if (!file) return NULL; 

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0'; 

    cJSON *json = cJSON_Parse(data);
    free(data);

    if (!json) return NULL;

    cJSON *jsonVariable = cJSON_GetObjectItem(json, variableName);
    char* variable = malloc(strlen(jsonVariable->valuestring) + 1);
    if (jsonVariable->string){
        strcpy(variable, jsonVariable->valuestring);
    }
    
    cJSON_Delete(json);
    return variable;
}

void saveCharVariable(char* variable, char* variableName){ 
    cJSON *json = NULL;
    FILE *file = fopen("assets/data.json", "r");
    long length = 0;
    char *data = NULL;

    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        data = (char *)malloc(length + 1);
        if (data != NULL) {
            fread(data, 1, length, file);
            data[length] = '\0';
            json = cJSON_Parse(data);
        }
        fclose(file);
        free(data);
    }
    
    if (json == NULL) { // Si le fichier n'existe pas, est vide ou contient un JSON invalide
        json = cJSON_CreateObject();
    }

    if (cJSON_HasObjectItem(json, variableName)) {
        cJSON_ReplaceItemInObject(json, variableName, cJSON_CreateString(variable));
    } else {
        cJSON_AddStringToObject(json, variableName, variable);
    }

    char *json_data = cJSON_Print(json);
    file = fopen("assets/data.json", "w");

    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        free(json_data);
        cJSON_Delete(json);
        return;
    }

    fprintf(file, "%s", json_data);
    fclose(file);

    free(json_data);
    cJSON_Delete(json);
}
