#include <string.h>

int select_bands(char* selection, int *amounts){
    if(strcmp(selection,"bb") == 0){
        int amt_array[4] = {12, 4, 2, 2};
        for (int i = 0; i < 4; i++) {
            amounts[i] = amt_array[i];
        }   
    }
    else if(strcmp(selection,"tb") == 0){
        int amt_array[4] = {4, 12, 4, 2};
        for (int i = 0; i < 4; i++) {
            amounts[i] = amt_array[i];
        }           
    }
    else if(strcmp(selection,"ab") == 0){
        int amt_array[4] = {2, 4, 8, 2};
        for (int i = 0; i < 4; i++) {
            amounts[i] = amt_array[i];
        }
    }
    else if(strcmp(selection,"sb") == 0){
        int amt_array[4] = {2, 2, 4, 12};
        for (int i = 0; i < 4; i++) {
            amounts[i] = amt_array[i];
        }
    }
    else if(strcmp(selection,"lp") == 0){
        int amt_array[4] = {6, 6, 2, 0};
        for (int i = 0; i < 4; i++) {
            amounts[i] = amt_array[i];
        }
    }
    else if(strcmp(selection,"hp") == 0){
        int amt_array[4] = {0, 0, 0, 6};
        for (int i = 0; i < 4; i++) {
            amounts[i] = amt_array[i];
        }
    }
    else {
        return 1; // no match found from presets
    }
    return 0; // success
}