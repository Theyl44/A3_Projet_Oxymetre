/*
 *      mesure.h
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include "define.h"

typedef struct{
    //sp02
    absorp buffer[3000];

    //pouls
    int rithmeSave;
    int rithmeCounter;

    int buffer_pouls[2];
    float buffer_acir;
    int bufferPouls2;
}param_mesure;

oxy mesureTest(char* filename);
void init_mesure(param_mesure* myMES);
oxy mesure(absorp myAbsorp, param_mesure* myMES);