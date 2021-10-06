/*
 *      fir.h
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include "define.h"

typedef struct{
    float FIR_TAPS[51]; /*! FIR TAPS*/
    absorp buffer[51];  /*! buffer des 50 dernieres valeur*/
} param_fir;

absorp firTest(char* filename);             //c'est la fonction qui sera appeler par le correcteur pour verifier nos codes
absorp fir(absorp x, param_fir* myFIR);     //elle est appeler pas la fonction firTest mais aussi par l'intégration c'est notre filtre quoi
void initfir(param_fir* myFIR);             //initialise nos variable avant le filtre : on a besoin d'un buffer de 50 valeur a 0 et les coef donné par le prof, et on va stocker cela dans un paramètre param_fir