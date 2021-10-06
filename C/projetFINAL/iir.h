/*
 *      iir.h
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include "define.h"

typedef struct{
    absorp x_1;     /*! buffer de la derniere valeur de x*/
    absorp y_1;     /*! buffer de la derniere valeur de y*/
} param_iir;

void initiir(param_iir* myIIR);               //initialise nos variable avant le filtre : on a besoin des buffer de x et y == 0 au debut et elle stocker ces valeurs dans un paramètre param_IIR
absorp iirTest(char* filename);               //c'est la fonction qui sera appeler par le correcteur pour verifier nos codes
absorp irr(absorp x, param_iir* myIRR);       //elle est appeler pas la fonction iirTest mais aussi par l'intégration c'est notre filtre quoi